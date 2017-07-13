/*
 * ComponentFactory.cpp
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

/* Standard library inclusions */
#include <iostream>

/* Boost inclusions */
#include <boost/system/system_error.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/library_info.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/filesystem/operations.hpp>

/* Project specific inclusions */
#include "Component/IComponent.h"
#include "Component/IComponentDesigner.h"

#include "Component/Factory.h"
#include "Component/Exception.h"

/* The whole Factory implementation belongs to Component namespace */
using namespace Component;

/* Plugin API namespace */
using namespace Plugin::API;

const boost::filesystem::path Factory::mPluginsPath( boost::filesystem::current_path().generic_string() + std::string( "/ComponentPlugins/" ) );

Factory::Factory( void )
	:	/* Load all the plugins from appropriate directory and store imported factory method aliases into the Plugin loaders list */
		mPluginLoaderCollection( this->loadComponentPlugins( this->mPluginsPath ) ),
		/* Load all component designers from component plugins */
		mComponentDesignerCollection( this->loadComponentDesigners( this->mPluginLoaderCollection ) )
{
	/* Debug: Component designers statistics */
#if true
	for( std::pair<const std::string, const std::shared_ptr<IComponentDesigner> &> ComponentDesignerCollectionItem : (this->mComponentDesignerCollection) )
	{
		std::cout << "--> Component designer: " << ( ComponentDesignerCollectionItem.second )->getCathegory() << "::" << ( ComponentDesignerCollectionItem.second )->getName() << std::endl;
	}
	std::cout << "Component Factory: " << ( this->mComponentDesignerCollection ).size() << " component designers in " << ( this->mPluginLoaderCollection ).size() << " plugin(s) loaded." << std::endl;
#endif
}

std::shared_ptr<IComponent> CORE_EXPORT Factory::constructComponent( const std::shared_ptr<IComponentDesigner> ComponentDesigner, const std::shared_ptr<Core::ParameterContainer> Specification ) const
{
	return( ComponentDesigner->constructComponent( Specification ) );
}

std::shared_ptr<IComponentDesigner> Factory::getComponentDesigner( const std::string Name ) const
{
	TComponentDesignersCollection::const_iterator tIterator = ( this->mComponentDesignerCollection ).find( Name );

	if( tIterator == (this->mComponentDesignerCollection).end() )
	{
		/* Throw an exception */
		BOOST_THROW_EXCEPTION( Exception::Factory::ComponentDesignerNotFound() << Core::Exception::Message( std::string( "Requested component designer: " ) + Name ) );
	}

	return( tIterator->second );
}

Factory::TPluginLoaderCollection Factory::loadComponentPlugins( const boost::filesystem::path ComponentPluginsPath  ) const
{
	TPluginLoaderCollection tPluginLoaderCollection;

	try
	{
		if(	/* Component plugin path exists... */
			( boost::filesystem::exists( ComponentPluginsPath ) ) &&
			/* ... and is a directory */
			( boost::filesystem::is_directory( ComponentPluginsPath ) )
		)
		{
			/* Iterate through all the entries in component plugins path */
			for( const boost::filesystem::directory_entry & Entry : boost::make_iterator_range( boost::filesystem::directory_iterator( ComponentPluginsPath ), {} ) )
			{
				/* Ignore symlinks */
				if( !boost::filesystem::is_symlink( Entry.path() ) )
				{
					/* Plugin loading error code used to check whether the plugin has been loaded correctly */
					boost::system::error_code tPluginLoadError;

					/* Try to load plugin shared library */
					boost::dll::shared_library tPlugin( Entry.path(), tPluginLoadError, boost::dll::load_mode::append_decorations );

					/* If there are no errors during plugin loading, save it */
					if( !tPluginLoadError )
					{
						/* Check whether plugin has appropriate factory method symbol */
						if( tPlugin.has("LoadPlugin") )
						{
							/* Import alias for LoadPlugin method and add it to plugin loaders list.
							 * Plugin loaders holds the references to all the DSOs */
#if true
							/* Import plugin loader method */
							std::function<TPluginLoaderMethod> tPluginLoaderMethod = boost::dll::import_alias<TPluginLoaderMethod>( std::move( tPlugin ), "LoadPlugin" );

							/* Get plugin pointer to work with it's API */
							std::shared_ptr<Plugin::API::IComponentPluginAPI> Plugin = tPluginLoaderMethod();

							/* Call some plugin method defined in API */
							std::cout << "Plugin loaded: " << Plugin->getPluginName() << " " << Plugin->getPluginVersion() << " (" << Entry.path() << ")" << std::endl;

							/* Add the plugin loader method into the list of loaders */
							/* IMPORTED SYMBOLS HOLD THE REFERENCE TO PLUGIN */
							tPluginLoaderCollection.push_back( tPluginLoaderMethod );

#else
							/* Add the plugin loader method into the list of loaders */
							/* IMPORTED SYMBOLS HOLD THE REFERENCE TO PLUGIN */
							tPluginLoaderCollection.push_back( boost::dll::import_alias<TPluginLoaderMethod>( std::move( tPlugin ), "LoadPlugin" ) );
#endif
						}
						else
						{
#if false
							std::cout << "Incompatible plugin found."<< " (" << Entry.path() << ")" << std::endl;
#endif
						}
					}
				}
			}
		}
	}
	/* Catch exceptions thrown by the filesystem */
	catch( const boost::filesystem::filesystem_error & FileSystemErrorException )
	{
		BOOST_THROW_EXCEPTION( Exception::Factory::PluginLoadingFailed() << Core::Exception::Message( FileSystemErrorException.what() ) );
	}
	/* Catch exceptions thrown by boost::dll import */
	catch( const boost::system::system_error & SystemErrorException )
	{
		BOOST_THROW_EXCEPTION( Exception::Factory::PluginLoadingFailed() << Core::Exception::Message( SystemErrorException.what() ) );
	}

	return( tPluginLoaderCollection );
}

Factory::TComponentDesignersCollection Factory::loadComponentDesigners( const Factory::TPluginLoaderCollection & PluginLoaderCollection ) const
{
	/* Construct local temporary component designers collection */
	TComponentDesignersCollection tComponentDesigners;

	/* Iterate through all the plugins */
	for( const std::function<TPluginLoaderMethod> & PluginLoader : PluginLoaderCollection )
	{
		/* Get plugin pointer to work with it's API */
		std::shared_ptr<IComponentPluginAPI> Plugin = PluginLoader();

		/* Get the list of all plugin's component designers */
		IComponentPluginAPI::TIComponentDesignerList tPluginComponentDesignersList = Plugin->getComponentDesigners();

		/* Iterate through all the component designers loaded previously */
		for( const std::shared_ptr<IComponentDesigner> & ComponentDesigner : tPluginComponentDesignersList )
		{
			/* Add component designer into the component designer collection */
			/* The value of iterator is shared_ptr to component designer */
			tComponentDesigners.insert( std::pair<std::string, const std::shared_ptr<IComponentDesigner> & >( ComponentDesigner->getName(), ComponentDesigner ) );
		}
	}

	/* Return filled local temporary component designers collection */
	return( tComponentDesigners );
}
