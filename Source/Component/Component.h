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

/* Project specific inclusions */
#include "Component/IComponent.h"
#include "Component/IComponentModel.h"

#include "Parameter/IParameter.h"
#include "Parameter/ParameterContainer.h"

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

namespace Component
{
#if false
	/* Forward declaration of ComponentTraits specialized for every component type */
	template<typename DERIVED_COMPONENT_TYPE>
	struct ComponentTraits;
#endif

	template<typename DERIVED_COMPONENT_TYPE>
	class CORE_NO_EXPORT Component
		: /* Component implements it's interface */
		  public IComponent
	{
	public:
#if false
		/* Parameter container type alias to designer traits */
		using TParameterContainer = typename ComponentTraits<DERIVED_COMPONENT_TYPE>::TParameterContainer;

		/* Component type alias to designer traits */
		using TComponent = typename ComponentTraits<DERIVED_COMPONENT_TYPE>::TComponent;
#endif

		virtual void update( void ) = 0;

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
				(SpecificationParameter.second)->connectUpdateSignal( std::bind( & Component<DERIVED_COMPONENT_TYPE>::update, this ) );
			}

			/* Link the specification to own parameter container */
			mParameters->link( Specification );
		}

		const std::string & getName( void ) const override final
		{
			return( this->mComponentName );
		}

		const std::string & getCathegory( void ) const override final
		{
			return( this->mComponentCathegory );
		}

		std::shared_ptr<TopoDS_Shape> getModel( void ) const
		{
			if( this->mComponentModel )
				return( this->mComponentModel );
			else
			{
				/* TODO: Throw an exception here --> Model is not yet constructed */
				/* TODO: Remove this temporary return statement */
				return( nullptr );
			}
		}

	protected:

		const std::shared_ptr<Core::ParameterContainer> & getParameters( void ) const
		{
			return( this->mParameters );
		}

	private:

		Component( void ) = delete;

		std::string mComponentName;
		std::string mComponentCathegory;

		std::shared_ptr<Core::ParameterContainer> mParameters;

		std::shared_ptr<TopoDS_Shape> mComponentModel;
	};
}

#endif /* CORE_COMPONENT_COMPONENT_H_ */
