/*
 * ComponentDesigner.h
 *
 *  Created on: 26. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_COMPONENTDESIGNER_H_
#define CORE_COMPONENT_COMPONENTDESIGNER_H_

/* Standard library inclusions */
#include <string>
#include <memory>

/* Project specific inclusions */
#include "Parameter/ParameterContainer.h"

#include "Component/IComponent.h"
#include "Component/IComponentDesigner.h"

#include "Component/Component.h"

namespace Component
{
	template<typename COMPONENT_TYPE>
	class ComponentDesigner
		:	public IComponentDesigner
	{
	public:
		/* Check whether COMPONENT_TYPE is compatible with IComponent interface */
		static_assert( std::is_base_of<IComponent, COMPONENT_TYPE>::value, "Component designer's component type must implement IComponent interface." );

		ComponentDesigner( std::string Name, std::string Cathegory )
			:	mName( Name ), mCathegory( Cathegory )
		{}

		const std::string & getName( void ) const override final
		{
			return( this->mName );
		}

		const std::string & getCathegory( void ) const override final
		{
			return( this->mCathegory );
		}

		std::unique_ptr<IComponent> constructComponent( const std::shared_ptr<Core::ParameterContainer> Specification ) const override final
		{
			std::unique_ptr<COMPONENT_TYPE> tComponent = std::make_unique<COMPONENT_TYPE>( std::move( Specification ) );

			/* Once the instance is existing, run the component construction task. Internally, it is run in the thread pool. */
			tComponent->requestUpdate();

			/* Return constructed component */
			return( std::move( tComponent ) );
		}

	protected:

	private:
		std::string mName;

		std::string mCathegory;
	};
}

#endif /* CORE_COMPONENT_COMPONENTDESIGNER_H_ */
