/*
 * Parameter_IdentificationGenerator.h
 *
 *  Created on: 2.6.2016
 *      Author: martin
 */

#ifndef CORE_PARAMETER_PARAMETERIDENTIFICATIONGENERATOR_H_
#define CORE_PARAMETER_PARAMETERIDENTIFICATIONGENERATOR_H_

/* Standard library inclusions */
#include <string>
#include <sstream>

/* Boost inclusions */
#include <boost/serialization/singleton.hpp>

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

namespace Core
{
	namespace Detail
	{
		/**
		 * @brief Parameter identification generator {SINGLETON}
		 *
		 * This class is based on boost::serialization::singleton implementation of singleton
		 * design pattern using CRTP idiom. An instance of ParameterIdentificationGenerator is
		 * instantiated during startup.
		 *
		 * The instance just holds unsigned integer value which is incremented every time the unique
		 * parameter identification is requested. This ensures unique name and symbol (matching together
		 * by sharing the same unique number) which is can be used by Parameter<> class while default
		 * instance construction (no specific symbol and name set, so defaults shall be used instead)
		 *
		 * Usage:
		 * @code{.cpp}
		 *
		 * ParameterIdentificationGenerator::ParameterIdentification UniqueIdentification;
		 *
		 * UniqueIdentification = ParameterIdentificationGenerator::get_mutable_instance().getUniqueParameterIdentification();
		 *
		 * std::string Name	( UniqueIdentification.mName );
		 * std::string Symol( UniqueIdentification.mSymbol );
		 * @endcode
		 */
		class CORE_NO_EXPORT ParameterIdentificationGenerator
			:	/* Derive boost SINGLETON using CRTP implementation */
				public boost::serialization::singleton<ParameterIdentificationGenerator>
		{
		public:
			/**
			 * @brief Parameter identification container {MESSENGER}
			 *
			 * Unique parameter identification container class holding just parameter name and symbol.
			 */
			struct ParameterIdentification
			{
				/**
				 * @brief Parameter identification container constructor
				 *
				 * Constructs container contents (Name, Symbol). As both container's member variables
				 * are declared as constants, the value can be assigned only once. So once constructed
				 * using this constructor, the data cannot be changed any more (principle of MESSENGER pattern)
				 */
				ParameterIdentification( const std::string & Symbol, const std::string & Name )
					:	mName( Name ),
						mSymbol( Symbol )
				{}

				/** @property std::string mName		Parameter identification name - default name + unique ID */
				const std::string mName;
				/** @property std::string mSymbol	Parameter identification symbol - default symbol + unique ID */
				const std::string mSymbol;
			};

			/**
			 * @brief Get unique parameter identification
			 *
			 * Gets unique ID owned by the singleton and constructs the default name and symbol using the unique ID.
			 *
			 * @returns ParameterIdentification Instance of parameter identification container
			 */
			ParameterIdentification getUniqueParameterIdentification( void )
			{
				/* Construct two temporary string streams to create name and symbol strings */
				std::stringstream Symbol, Name;

				/* Get unique ID to be build in the name and symbol */
				unsigned int UniqueNumber = (this)->getUniqueSerialNumber();

				/* Construct the identification... */
				Symbol << "P" << UniqueNumber;
				Name << "Parameter" << UniqueNumber;

				/* ...and return it stored in proper container */
				return( ParameterIdentification( Symbol.str(), Name.str() ) );
			}

		private:
			/**
			 * @brief Get unique serial number (ID)
			 *
			 * @returns usigned int	Unique ID incremented once the new ID is requested
			 */
			unsigned int getUniqueSerialNumber( void )
			{
				return( mSerialNumber++ );
			}

			/** @property unsigned int mSerialNumber		Unsigned integer value incremented everytime the new ID is requested */
			unsigned int	mSerialNumber = 0;
		};

	} /* namespace Detail */
} /* namespace Core */

#endif /* CORE_PARAMETER_PARAMETERIDENTIFICATIONGENERATOR_H_ */
