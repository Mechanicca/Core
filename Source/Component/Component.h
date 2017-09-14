/*
 * Component.h
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_COMPONENT_H_
#define CORE_COMPONENT_COMPONENT_H_

/* Standard library inclusions */
#include <memory>

/* OpenCascade inclusions */
#include "TopoDS_Shape.hxx"

/* Project specific inclusions */
#include "Component/IComponent.h"
#include "Component/IComponentModel.h"
#include "Component/Exception.h"
#include "Parameter/IParameter.h"
#include "Parameter/ParameterContainer.h"
#include "ThreadPool/ThreadPool.h"

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

#define DEBUG_CONSOLE_OUTPUT	true

namespace Component
{
	/* Forward declaration of ComponentTraits specialized for every component type */
	template<typename DERIVED_COMPONENT_TYPE>
	struct ComponentTraits;

	template<typename DERIVED_COMPONENT_TYPE>
	class CORE_NO_EXPORT Component
		: /* Component implements it's interface */
		  public IComponent
	{
	public:
		/**
		 * @brief Component Update Request
		 *
		 * The goal is to run the update() method in separate task to avoid blocking of main thread. The update() method
		 * is private and is, in fact, a receipt what to update in which order.
		 */
		void requestUpdate( void ) override final
		{
			/* Prepare function pointer to update() method which is about to run in a ThreadPool */
			std::function<void ( void )> f = std::bind( & Component<DERIVED_COMPONENT_TYPE>::update, this );

			/* Run update() method in ThreadPool */
			Core::ThreadPool::get_mutable_instance().add( f );
		}

		const std::string & getName( void ) const override final
		{
			return( this->mComponentName );
		}

		const std::string & getCathegory( void ) const override final
		{
			return( this->mComponentCathegory );
		}

	protected:

		/**
		 * @brief Component constructor
		 *
		 * Initializes internal member variables, connects Qt signal 'requestUpdate'
		 */
		Component( const std::shared_ptr<Core::ParameterContainer> Specification )
			:	/* Create parameter container */
				mParameters( Core::ParameterContainer::construct() ),
				/* Initially, the component model smart pointer can be set to nullptr because the instance
				 * is created as temporary in update() member method. Once the construction is finished there,
				 * the temporary model is swapped with mComponentModel thus becomes valid. */
				mComponentModel( nullptr )
		{
			/* Connect all the specification parameter's update signals to component construction method.
			 * Once any of the specification parameters is updated, the whole component is recalculated.
			 */
			for( const std::pair<const Core::ParameterContainer::TParameterContainerKey, std::shared_ptr<Core::IParameter>> & SpecificationParameter : (* Specification) )
			{
				/* ...connect specification parameter to SpecificationUpdated() slot */
				(SpecificationParameter.second)->connectUpdateSignal( std::bind( & Component<DERIVED_COMPONENT_TYPE>::requestUpdate, this ) );
			}

			/* Link the specification to own parameter container */
			mParameters->link( Specification );
		}

		const std::shared_ptr<Core::ParameterContainer> & getParameters( void ) const
		{
			return( this->mParameters );
		}

	private:

		Component( void ) = delete;

		void update( void )
		{
#if DEBUG_CONSOLE_OUTPUT
			std::cout << "Starting component model update" << std::endl;

			/* Start measuring time... */
			auto Start = std::chrono::steady_clock::now();
#endif

			std::unique_ptr<IComponentModel> tComponentModel = typename ComponentTraits<DERIVED_COMPONENT_TYPE>::TComponentModelConstructor()();

			try
			{
				/* Construct updated model referenced by temporary component model pointer */
				std::future<std::unique_ptr<TopoDS_Shape>> tModelShapeFuture = tComponentModel->constructModel( this->getParameters() );

				/* Wait for the model to be fully constructed */
				tModelShapeFuture.wait();

				/* Once the model shape is fully updated, save it. */
				this->mComponentModel = tModelShapeFuture.get();
			}
			catch( const Exception::Design::ModelConstructionFailed & Exception )
			{
				/* TODO: Improve error handling. It should not just to print out the error message... */
				std::cout << "Component cannot be constructed. " << Exception.what() << std::endl;
			}

#if DEBUG_CONSOLE_OUTPUT
			auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - Start );

			std::cout << "Component updated in " << Duration.count() << " milliseconds using Thread Pool having " << Core::ThreadPool::get_mutable_instance().threadCount() << " worker threads." << std::endl;
#endif
		}

		std::string mComponentName;
		std::string mComponentCathegory;

		std::shared_ptr<Core::ParameterContainer> mParameters;

		std::shared_ptr<TopoDS_Shape> mComponentModel;
	};
}

#undef DEBUG_CONSOLE_OUTPUT

#endif /* CORE_COMPONENT_COMPONENT_H_ */
