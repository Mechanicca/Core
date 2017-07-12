/*
 * Data_DesignRulesDBConnection.h
 *
 *  Created on: 14.6.2016
 *      Author: martin
 */

#ifndef SOURCE_CORE_DATA_DATA_DESIGNRULESDBCONNECTOR_H_
#define SOURCE_CORE_DATA_DATA_DESIGNRULESDBCONNECTOR_H_

/* Standard library inclusions */
#include <memory>

/* Boost inclusions */
#include <boost/serialization/singleton.hpp>

/* SQLiteCpp library */
#include "SQLiteCpp/SQLiteCpp.h"

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

#include "Exception/Exception.h"

namespace Core
{
	class CORE_EXPORT DesignRulesDBConnector
		:	public boost::serialization::singleton<DesignRulesDBConnector>
	{
	public:

		struct Exception
		{
			/**
			 * @brief Generic DesignRules DB Connector exception
			 */
			struct Generic : virtual Core::Exception::Generic {};

			/**
			 * @brief DesignRules database not connected exception
			 *
			 * TODO: Add description when used
			 */
			struct DesignRulesDBNotConnected : virtual Generic {};
		};

		/**
		 * @brief Database connection status check method
		 *
		 * This method can be used to check whether the database is connected or not.
		 *
		 * @retval true		Design rules database is connected and can be used.
		 * @retval false	Design rules database is disconnected.
		 *
		 * Example usage:
		 * @code{.cpp}
		 *      if( DesignRulesDBConnector::get_mutable_instance().isConnected() )
		 *      {
		 *     		// DB can be used here
		 *      }
		 *      else
		 *      {
		 *      	// DB not usable as is disconnected!
		 *      }
		 * @endcode
		 *
		 */
		bool isConnected( void );

		operator SQLite::Database & ( void )
		{
			return( (* mDesignRulesDB) );
		}

	protected:
		/**
		 * @brief Design rules constructor
		 *
		 * This constructor is made protected in order to follow the SINGLETON design pattern principle
		 * realized by boost::serialization::singleton and CRTP idiom.
		 *
		 * The main goal of this method is to establish database connection to design rules. In general
		 * it tries to open the database file in a location derived from application binary working directory.
		 * Once the connection is established, mIsConnected property is set. This status can be read out using
		 * public IsConnected() method.
		 *
		 * Thanks to boost::serialization::singleton and CRTP, DesignRulesDBConnector singleton is instantiated at
		 * pre-execution time so it can be used directly.
		 *
		 */
		DesignRulesDBConnector( void );

	private:
		/* Booloean value holding database connection status */
		bool 								mIsConnected;
		/* SQLite database filename */
		std::string							mFileName;
		/* Smart pointer to design rules database */
		std::shared_ptr<SQLite::Database>	mDesignRulesDB;
	};

} /* namespace Base */

#endif /* SOURCE_CORE_DATA_DATA_DESIGNRULESDBCONNECTOR_H_ */
