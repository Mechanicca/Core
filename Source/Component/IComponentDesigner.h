/*
 * IComponentDesigner.h
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_ICOMPONENTDESIGNER_H_
#define CORE_COMPONENT_ICOMPONENTDESIGNER_H_

/* Standard library inclusions */
#include <string>
#include <memory>

/* Project specific inclusions */

/* Forward declarations */
namespace Core
{
	class ParameterContainer;
}

namespace Component
{
	/* Component interface forward declaration */
	class IComponent;

	class IComponentDesigner
	{
	public:
		virtual const std::string & getName( void ) const = 0;

		virtual const std::string & getCathegory( void ) const = 0;

		virtual std::unique_ptr<IComponent> constructComponent( const std::shared_ptr<Core::ParameterContainer> Specification ) const = 0;

	protected:
		virtual ~IComponentDesigner( void ) = default;

	private:
	};
}

#endif /* CORE_COMPONENT_ICOMPONENTDESIGNER_H_ */
