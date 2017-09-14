/*
 * ComponentModel.h
 *
 *  Created on: 15. 5. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_COMPONENTMODEL_H_
#define CORE_COMPONENT_COMPONENTMODEL_H_

/* Standard library inclusions */
#include <memory>
#include <future>
#include <functional>

/* OpenCascade inclusions */
#include <TopoDS_Shape.hxx>

/* Project specific inclusions */
#include "ThreadPool/ThreadPool.h"
#include "Component/IComponentModel.h"

namespace Base
{
	class ParameterContainer;
}

namespace Component
{
	class ComponentModel
		:	public IComponentModel
	{
	public:

	protected:
		ComponentModel( void )
		{}

		virtual std::unique_ptr<TopoDS_Shape> constructComponentModel( const std::shared_ptr<Core::ParameterContainer> Parameters ) const = 0;

		/** @brief Construct Model
		 * This generic method is just used to run the constructCompoentModel in threadpool. The constructComponentModel() method must be implemented
		 * in all the component model classes which derive this base. This layout ensures all the models are constructed in own threads.
		 */
		std::future<std::unique_ptr<TopoDS_Shape>> constructModel( const std::shared_ptr<Core::ParameterContainer> Parameters ) const override final
		{
			/* Prepare function pointer to constructModelModifier() method */
			std::function<std::unique_ptr<TopoDS_Shape>(const std::shared_ptr<Core::ParameterContainer>)> f = std::bind( & ComponentModel::constructComponentModel, this, std::placeholders::_1 );

			/* Run the task in thread pool and return the std::future of the result */
			return( Core::ThreadPool::get_mutable_instance().add( f, Parameters ) );
		}

	private:

	};

	template<typename COMPONENT, typename... OTHER_MODIFIERS>
	struct ComponentModelConstructor
	{
		std::unique_ptr<IComponentModel> operator() ( void )
		{
			return( std::make_unique<COMPONENT>() );
		}
	};

	template<typename COMPONENT, typename MODIFIER, typename... OTHER_MODIFIERS>
	struct ComponentModelConstructor<COMPONENT, MODIFIER, OTHER_MODIFIERS ...>
	{
		std::unique_ptr<IComponentModel> operator() ( void )
		{
			return( std::make_unique<MODIFIER>( ComponentModelConstructor<COMPONENT, OTHER_MODIFIERS ...>()() ) );
		}
	};
}

#endif /* CORE_COMPONENT_COMPONENTMODEL_H_ */
