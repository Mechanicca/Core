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
		static_assert( std::is_base_of<IComponent, COMPONENT_TYPE>::value, "Component designer's component type must implement IComponentDesigner interface." );

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

		const std::shared_ptr<IComponent> constructComponent( const std::shared_ptr<Core::ParameterContainer> Specification ) const override final
		{
			/* Instantiate the component using it's constructor */
			std::shared_ptr<COMPONENT_TYPE> tComponent = std::make_shared<COMPONENT_TYPE>( std::move( Specification ) );

			/* FIXME: This is most probably very time consuming task --> should run in a separate thread? */
			/* Once the instance is existing, run the component construction task */
			tComponent->update();

			/* Return constructed component */
			return( tComponent );
		}

	protected:

	private:
		std::string mName;
		std::string mCathegory;
	};
}

#endif /* CORE_COMPONENT_COMPONENTDESIGNER_H_ */
