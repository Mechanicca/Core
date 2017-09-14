/*
 * ComponentModifier.h
 *
 *  Created on: 15. 5. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_COMPONENTMODELMODIFIER_H_
#define CORE_COMPONENT_COMPONENTMODELMODIFIER_H_

/* Standard library inclusions */
#include <memory>
#include <future>
#include <functional>

/* OpenCascade inclusions */
#include <TopoDS_Shape.hxx>

/* Project specific inclusions */
#include "Component/IComponentModel.h"
#include "ThreadPool/ThreadPool.h"

namespace Base
{
	class ParameterContainer;
}

namespace Component
{
	/** @brief Component modifier [DECORATOR]
	 *
	 *  ComponentModifier is common base for all the concrete component modifiers
	 *  (corresponds to Decorator class in http://www.oodesign.com/decorator-pattern.html)
	 *
	 *  Maintains a reference to a Component object and defines an interface that conforms to Component's interface
	 */
	class ComponentModelModifier
		:	public IComponentModel
	{
	public:

	protected:
		ComponentModelModifier( const std::shared_ptr<IComponentModel> ComponentModel )
			:	mModifiedComponentModel( ComponentModel )
		{}

		virtual std::unique_ptr<TopoDS_Shape> constructModelModifier( const std::shared_ptr<Core::ParameterContainer> Parameters ) const = 0;

		virtual std::unique_ptr<TopoDS_Shape> applyModifier( const std::shared_ptr<TopoDS_Shape> ComponentShape, const std::shared_ptr<TopoDS_Shape> ModifierShape ) const = 0;

	private:
		std::future<std::unique_ptr<TopoDS_Shape>> constructModel( const std::shared_ptr<Core::ParameterContainer> Parameters ) const override final
		{
			/* This must be called first in order to keep proper construction flow.
			 * This call shall not be processed by the thread pool as all the time consuming operations are added
			 * to the thread pool inside this constructModel() method */
			std::future<std::unique_ptr<TopoDS_Shape>> ComponentModelFuture = mModifiedComponentModel->constructModel( Parameters );

			/* Prepare function pointer to constructModelModifier() method */
			std::function<std::unique_ptr<TopoDS_Shape>(const std::shared_ptr<Core::ParameterContainer>)> tConstructModelModifierFunction = std::bind( & ComponentModelModifier::constructModelModifier, this, std::placeholders::_1 );

			/* Run component model modifier construction in ThreadPool and save the future result - constructed
			 * component model modifier referenced by shared_ptr to it's interface */
			std::future<std::unique_ptr<TopoDS_Shape>> ComponentModelModifierFuture = Core::ThreadPool::get_mutable_instance().add( tConstructModelModifierFunction, Parameters );

			/* Prepare applyModifier() function pointer to be enqueued in thread pool */
			std::function<std::unique_ptr<TopoDS_Shape>( const std::shared_ptr<TopoDS_Shape>, const std::shared_ptr<TopoDS_Shape> )> tApplyFunction;

			tApplyFunction = std::bind( & ComponentModelModifier::applyModifier, this, std::placeholders::_1, std::placeholders::_2 );

			/* Run the applyModifier() method and return the future result of it */
			return( Core::ThreadPool::get_mutable_instance().add( tApplyFunction, std::move<std::shared_ptr<TopoDS_Shape>>( ComponentModelFuture.get() ), std::move<std::shared_ptr<TopoDS_Shape>>( ComponentModelModifierFuture.get() ) ) );
		}

	protected:
		/* Modified component model reference as required by [DECORATOR] design pattern */
		std::shared_ptr<IComponentModel> mModifiedComponentModel;

	private:

	};
}

#endif /* CORE_COMPONENT_COMPONENTMODELMODIFIER_H_ */
