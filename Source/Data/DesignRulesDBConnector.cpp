/*
 * Data_DesignRulesDBConnection.cpp
 *
 *  Created on: 14.6.2016
 *      Author: martin
 */

/* Standard library inclusions */
#include <sstream>

/* Boost inclusions */
#include <boost/filesystem.hpp>

/* Project specific inclusions */
#include "DesignRulesDBConnector.h"

using namespace Core;

DesignRulesDBConnector::DesignRulesDBConnector( void )
	: 	/* Initially, let's make the connection invalid */
		mIsConnected( false )
{
	/* Get current application working directory */
	mFileName = boost::filesystem::current_path().generic_string();
	/* Append database file name */
	/* TODO: Manage database file name not to be hard-coded here --> Propagate it from cmake definitions somehow... */
	mFileName.append( "/DesignRules.db" );

	try
	{
		/* Connect to the database file and get the handle to it */
		mDesignRulesDB = std::shared_ptr<SQLite::Database>( new SQLite::Database( mFileName ) );

		/* Once this statement is reached, the DB is connected */
		mIsConnected = true;
	}
	/* Catch SQLite exception once something went wrong during database connection */
	catch ( const SQLite::Exception & Exception )
	{
		/* Save connection result */
		mIsConnected = false;

		/* Temporary stringstream used during error message construction */
		std::stringstream ErrorMessage;

		/* Construct error message */
		ErrorMessage << "DesignRulesDBConnector: Database not connected. (" << Exception.what() << ", DB Filename: '" << mFileName << "')" << std::endl;

		/* HINT: Once this error is reported during development in Eclipse, try to check run/debug configurations working directory setting */
		/* Throw DesignRulesNotConnected exception */
		BOOST_THROW_EXCEPTION( Exception::DesignRulesDBNotConnected() << Core::Exception::Message( ErrorMessage.str() ) );
	}
}

bool DesignRulesDBConnector::isConnected( void )
{
	return( mIsConnected );
}
