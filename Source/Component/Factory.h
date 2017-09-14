/*
 * ComponentFactory.h
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_FACTORY_H_
#define CORE_COMPONENT_FACTORY_H_

/* Standard library inclusions */
#include <list>
#include <map>
#include <memory>
#include <functional>

/* Boost inclusions */
#include <boost/serialization/singleton.hpp>
//#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

/* Project specific inclusions */
#include "Component/IComponent.h"
#include "Component/IComponentDesigner.h"
#include "Component/IComponentPluginAPI.h"

#include "Component/Exception.h"

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

/* Forward declarations */
namespace Core
{
	class ParameterContainer;
}

namespace Component
{
	class CORE_EXPORT Factory
		:	/* Derive boost SINGLETON using CRTP */
			public boost::serialization::singleton<Factory>
	{
	public:

		/* Thanks to composite design pattern, the smart pointer returned may point to
		 * simple component (instance of Component class) or to assembly (instance of Assembly class).
		 * It depends on Component Designer what it creates
		 */
		std::unique_ptr<IComponent> CORE_EXPORT constructComponent( const std::shared_ptr<IComponentDesigner> ComponentDesigner, const std::shared_ptr<Core::ParameterContainer> Specification ) const;

		std::shared_ptr<IComponentDesigner> getComponentDesigner( const std::string Name ) const;

	protected:
		Factory( void );

	private:

		/* TYPE DEFINITIONS */
		typedef std::shared_ptr<Plugin::API::IComponentPluginAPI> (TPluginLoaderMethod)( void );

		using TPluginLoaderCollection = std::list<std::function<TPluginLoaderMethod>>;

		using TComponentDesignersCollection = std::map<std::string, const std::shared_ptr<IComponentDesigner> >;

		/* METHODS */
		TPluginLoaderCollection loadComponentPlugins( const boost::filesystem::path ComponentPluginsPath ) const;

		TComponentDesignersCollection loadComponentDesigners( const TPluginLoaderCollection & PluginLoaders ) const;

		/* VARIABLES */
		TPluginLoaderCollection 				mPluginLoaderCollection;

		TComponentDesignersCollection 			mComponentDesignerCollection;

		static const boost::filesystem::path	mPluginsPath;
	};
}

#endif /* CORE_COMPONENT_FACTORY_H_ */
