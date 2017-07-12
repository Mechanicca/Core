
/* Standard library inclusions */
#include <iterator>
#include <algorithm>

#include <iostream>

/* Project specific inclusions */
#include "ThreadPool/ThreadPool.h"

using namespace Core;

CORE_EXPORT ThreadPool::ThreadPool( void )
:	ThreadCount( std::thread::hardware_concurrency() ),
	threadsWaiting(0),
	terminate(false),
	paused(false)
{
	std::cout << "Starting ThreadPool running " << ThreadCount << " worker threads." << std::endl;

	// prevent potential reallocation, thereby screwing up all our hopes and dreams
	threads.reserve( ThreadCount );
	std::generate_n(std::back_inserter(threads), ThreadCount, [this]() { return std::thread{threadTask, this}; });
}

CORE_EXPORT ThreadPool::~ThreadPool()
{
	clear();

	// tell threads to stop when they can
	terminate = true;
	jobsAvailable.notify_all();

	// wait for all threads to finish
	for(auto& t : threads)
	{
		if(t.joinable())
			t.join();
	}
}

CORE_EXPORT std::size_t ThreadPool::threadCount() const
{
	return threads.size();
}

CORE_EXPORT std::size_t ThreadPool::waitingJobs() const
{
	std::lock_guard<std::mutex> jobLock(jobsMutex);
	return jobs.size();
}

CORE_EXPORT ThreadPool::Ids ThreadPool::ids() const
{
	Ids ret(threads.size());

	std::transform(threads.begin(), threads.end(), ret.begin(), [](const std::thread& t) { return t.get_id(); });

	return ret;
}

CORE_EXPORT void ThreadPool::clear()
{
	std::lock_guard<std::mutex> lock{jobsMutex};

	while(!jobs.empty())
		jobs.pop();
}

CORE_EXPORT void ThreadPool::pause(bool state)
{
	paused = state;

	if(!paused)
		jobsAvailable.notify_all();
}

CORE_EXPORT void ThreadPool::wait()
{
	// we're done waiting once all threads are waiting
	while(threadsWaiting != threads.size());
}

CORE_EXPORT void ThreadPool::threadTask(ThreadPool* pool)
{
	// loop until we break (to keep thread alive)
	while(true)
	{
		// if we need to finish, let's do it before we get into
		// all the expensive synchronization stuff
		if(pool->terminate)
			break;

		std::unique_lock<std::mutex> jobsLock{pool->jobsMutex};

		// if there are no more jobs, or we're paused, go into waiting mode
		if(pool->jobs.empty() || pool->paused)
		{
			++pool->threadsWaiting;
			pool->jobsAvailable.wait(jobsLock, [&]()
			{
				return pool->terminate || !(pool->jobs.empty() || pool->paused);
			});
			--pool->threadsWaiting;
		}

		// check once more before grabbing a job, since we want to stop ASAP
		if(pool->terminate)
			break;

		// take next job
		auto job = std::move(pool->jobs.front());
		pool->jobs.pop();

		jobsLock.unlock();

		job();
	}
}
