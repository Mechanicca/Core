/*
 * IComponentPluginAPI.h
 *
 *  Created on: 21. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_ICOMPONENTPLUGINAPI_H_
#define CORE_COMPONENT_ICOMPONENTPLUGINAPI_H_

/* Standard library inclusions */
#include <string>
#include <list>
#include <memory>

namespace Component
{
	/* Component designer interface forward declaration */
	class IComponentDesigner;

	namespace Plugin
	{
		namespace API
		{
			class IComponentPluginAPI
			{
			public:
				using TIComponentDesignerList = std::list<std::shared_ptr<IComponentDesigner>>;

				virtual const std::string & getPluginName( void ) const = 0;

				virtual const std::string & getPluginVersion( void ) const = 0;

				virtual const TIComponentDesignerList getComponentDesigners( void ) const = 0;

			protected:
				virtual ~IComponentPluginAPI( void ) = default;

			private:

			};
		}
	}
}

#endif /* CORE_COMPONENT_ICOMPONENTPLUGINAPI_H_ */
