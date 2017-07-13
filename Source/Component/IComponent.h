/*
 * IComponent.h
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_ICOMPONENT_H_
#define CORE_COMPONENT_ICOMPONENT_H_

/* Standard library inclusions */
#include <string>

/* Forward declarations */
class TopoDS_Shape;

namespace Component
{
	class IComponent
	{
	public:
		virtual const std::string & getName( void ) const = 0;

		virtual const std::string & getCathegory( void ) const = 0;

		virtual std::shared_ptr<TopoDS_Shape> getModel( void ) const = 0;

	protected:
		virtual ~IComponent( void ) = default;

	private:

	};
}

#endif /* CORE_COMPONENT_ICOMPONENT_H_ */
