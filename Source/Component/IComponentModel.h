/*
 * IComponentModel.h
 *
 *  Created on: 15. 5. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_ICOMPONENTMODEL_H_
#define CORE_COMPONENT_ICOMPONENTMODEL_H_

/* Standard library inclusions */
#include <memory>
#include <future>

/* Forward declarations */
class TopoDS_Shape;

namespace Core
{
	class ParameterContainer;
}

namespace Component
{
	class IComponentModel
	{
	public:
		virtual std::future<std::unique_ptr<TopoDS_Shape>> constructModel( const std::shared_ptr<Core::ParameterContainer> Parameters ) const = 0;

		virtual ~IComponentModel( void ) = default;

	private:

	};
}

#endif /* CORE_COMPONENT_ICOMPONENTMODEL_H_ */
