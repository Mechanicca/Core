/*
 * ThreadPool.h
 *
 *  Created on: 8. 5. 2017
 *      Author: martin
 */

#ifndef CORE_THREADPOOL_THREADPOOL_H_
#define CORE_THREADPOOL_THREADPOOL_H_

/* Standard library inclusions */
#include <atomic>
#include <mutex>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <condition_variable>
#include <queue>

/* Boost inclusions */
#include <boost/serialization/singleton.hpp>

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

/* NOTE: The ThreadPool implementation is forked from: https://github.com/dabbertorres/ThreadPool */

namespace Core
{
	class CORE_EXPORT ThreadPool
		:	/* ThreadPool is a SINGLETON */
			public boost::serialization::singleton<ThreadPool>
	{
	public:
		using Ids = std::vector<std::thread::id>;

		// add a function to be executed, along with any arguments for it
		template<typename FUNCTION, typename... ARGUMENTS>
		auto add(FUNCTION&& Function, ARGUMENTS&&... Arguments) -> std::future<typename std::result_of<FUNCTION(ARGUMENTS...)>::type>;

		// returns number of threads being used
		std::size_t threadCount() const;

		// returns the number of jobs waiting to be executed
		std::size_t waitingJobs() const;

		// returns a vector of ids of the threads used by the ThreadPool
		Ids ids() const;

		// clears currently queued jobs (jobs which are not currently running)
		void clear();

		// pause and resume job execution. Does not affect currently running jobs
		void pause(bool state);

		// blocks calling thread until job queue is empty
		void wait();

	protected:
#if false
		using ThreadCount = std::integral_constant<std::size_t, 4>;
#endif

		// starts threadCount threads, waiting for jobs
		// may throw a std::system_error if a thread could not be started
		ThreadPool( void );

		// clears job queue, then blocks until all threads are finished executing their current job
		~ThreadPool();

	private:
		using Job = std::function<void()>;

		// function each thread performs
		static void threadTask(ThreadPool* pool);

		const unsigned int ThreadCount;

		std::queue<Job> jobs;
		mutable std::mutex jobsMutex;

		// notification variable for waiting threads
		std::condition_variable jobsAvailable;

		std::vector<std::thread> threads;
		std::atomic<std::size_t> threadsWaiting;

		std::atomic<bool> terminate;
		std::atomic<bool> paused;
	};

	template<typename FUNCTION, typename... ARGUMENTS>
	auto ThreadPool::add( FUNCTION&& Function, ARGUMENTS&&... Arguments ) -> std::future<typename std::result_of<FUNCTION(ARGUMENTS...)>::type>
	{
		using PackedTask = std::packaged_task<typename std::result_of<FUNCTION(ARGUMENTS...)>::type()>;

		auto task = std::make_shared<PackedTask>(std::bind(std::forward<FUNCTION>(Function), std::forward<ARGUMENTS>(Arguments)...));

		// get the future to return later
		auto ret = task->get_future();

		{
			std::lock_guard<std::mutex> lock{jobsMutex};
			jobs.emplace([task]() { (*task)(); });
		}

		// let a waiting thread know there is an available job
		jobsAvailable.notify_one();

		return ret;
	}
}

#endif /* CORE_THREADPOOL_THREADPOOL_H_ */
