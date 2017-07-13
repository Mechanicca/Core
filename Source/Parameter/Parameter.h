/*
 * Parameter.h
 *
 *  Created on: 16.6.2015
 *      Author: martin
 */

#ifndef CORE_PARAMETER_PARAMETER_H_
#define CORE_PARAMETER_PARAMETER_H_

/* Standard library inclusions */
#include <limits>
#include <string>
#include <iostream>
#include <type_traits>

/* Boost inclusions */
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/units/cmath.hpp>
#include <boost/units/is_unit.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/signals2.hpp>

/* Project specific inclusions */
#include "Parameter/IParameter.h"
#include "Parameter/IParameterTagCollection.h"
#include "Parameter/Exception.h"
#include "Parameter/ParameterIdentificationGenerator.h"
#include "Data/DesignRulesDBConnector.h"

/* Shared library support */
#include "Core_Export.h"

/* As Core_Export.h header is generated during build, the required CORE_EXPORT
 * definition might not exist due to missing header file. In order to prevent
 * syntax errors cause by undefined CORE_EXPORT, define temporary blank one */
#ifndef CORE_EXPORT
	#define CORE_EXPORT
	#define CORE_NO_EXPORT
#endif

using namespace boost::units;

namespace Core
{
	/* TODO: Try to find something more elegant --> static constant in separate module Parameter_Constants.h + Parameter_Constants.cpp ? */
	/* TODO: std::integral_constant ? */
	#define PARAMETER_DEFAULT_VALUE		0

	/**
	 * @brief Parameter class
	 *
	 * @tparam UNIT	Boost::Units::Quantity unit type
	 * @tparam DATA	POD data type to be used for parameter's quantity (defaults to 'double')
	 */
	template<typename UNIT, typename DATA = double>
	class Parameter
		:	/* Derive from Boost::Units::Quantity */
			public boost::units::quantity<UNIT, DATA>,
			/* Derive from Generic Parameter which is a common base for all parameter types */
			public IParameter
	{
		/* Check UNIT */
		static_assert( std::is_base_of<boost::mpl::true_, typename boost::units::is_unit<UNIT>>::value, "Parameter unit type must be of boost::units::unit type." );
		/* Check DATA */
		static_assert( std::is_arithmetic<DATA>::value, "DATA must be arithmetic type (integral or floating type)" );

	public:
		/**
		 * @typedef Parameter type definitions
		 *
		 * The Quantity is the type of boost::units::quantity overtaking Parameter template parameters
		 * UNIT and DATA defined in TData
		 */
		using TUnit = UNIT;

		using TData = DATA;

		using TQuantity = typename boost::units::quantity<TUnit, TData>::this_type;

		using TParameter = Parameter<TUnit, TData>;

		/**
		 * @brief Parameter default constructor
		 *
		 * Initializes all member variables ot it's default values.
		 */
		Parameter( void ) noexcept
			:	/* TODO: Change default initialization value */
				Parameter( TQuantity::from_value( TData( PARAMETER_DEFAULT_VALUE ) ) )
		{}

		/**
		 * @brief Parameter TQuantity copy constructor
		 *
		 * Constructs the Parameter by Quantity only. The other member variables are initialized
		 * to default values. Parameter identification is taken from ParameterIdentificationGenerator to
		 * ensure the default symbol and name is unique.
		 *
		 * @param [in] TQuantity Quantity		Reference to TQuantity representing Parameter value
		 */
		Parameter( const TQuantity & Quantity ) noexcept
			:	/* Call private constructor accepting UniqueParameterIdentification provided by ParameterIdentificationGenerator */
				Parameter(
					/* Get parameter identification */
					Core::Detail::ParameterIdentificationGenerator::get_mutable_instance().getUniqueParameterIdentification(),
					Quantity )
		{}

		/**
		 * @brief Parameter constructor taking symbol and name only
		 *
		 * @param [in] Symbol	Symbol string to be used during construction
		 * @param [in] Name		Name string to be used during construction
		 */
		Parameter( const std::string & Symbol, const std::string & Name ) noexcept
			:	/* TODO: Change default initialization value */
				Parameter( Symbol, Name, TQuantity::from_value( TData( PARAMETER_DEFAULT_VALUE ) ) )
		{}

		/**
		 * @brief Parameter constructor taking symbol, and quantity
		 *
		 * This is the full-fat constructor standing at the end of constructor chain
		 *
		 * @param [in] Symbol		Symbol string to be used during construction
		 * @param [in] Name			Name string to be used during construction
		 * @param [in] Quantity		Parameter quantity understood to be Parameter value
		 */
		Parameter( const std::string & Symbol, const std::string & Name, const TQuantity & Quantity ) noexcept
			:	/* Initialize quantity first */
				TQuantity::quantity( Quantity ),
				/* Setup Parameter identification */
				mName( Name ),
				mSymbol( Symbol ),
				/* Construct update signal */
				mUpdateSignal( std::make_shared<UpdateSignalType>() )
		{
			/* Setup Parameter limits */
			(this)->setDefaultLimits();
			/* Setup Default value */
			(this)->setDefaultValue( Quantity );
		}

		/**
		 * @brief Defautl copy constructor
		 *
		 * @param [in] AnotherParam		Parameter instance to be copied
		 */
		Parameter( const Parameter & AnotherParam ) noexcept( false ) = default;

		/**
		 * @brief Construct parameter using DesignRules
		 *
		 * @param [in] ConstrainedParameterIdentification	Parameter identification is DesignRules DB
		 */
		template<typename PARAMETER_TAG>
		Parameter( PARAMETER_TAG ) noexcept( false )
			:	/* Load all the parameter properties from DesignRules DB but set the initial value to something invalid */
				/* FIXME: Instance if PARAMETER_TAG is created here just for forwarding... */
				Parameter( PARAMETER_TAG(), TQuantity::from_value( TData( PARAMETER_DEFAULT_VALUE ) ) )
		{
			/* Check whether PARAMETER_TAG is compatible with IParameterTag interface */
			static_assert( std::is_base_of<IParameterTagCollection::IParameterTag, PARAMETER_TAG>::value, "Parameter TAG must be derived from IParameterTag." );

			/* Reset parameter value to it's default as defined in DesignRules DB */
			(this)->resetToDefault();
		}

		/**
		 * @brief Construct parameter using DesignRules
		 *
		 * @param [in] ConstrainedParameterIdentification	Parameter identification is DesignRules DB
		 */
		template<typename PARAMETER_TAG>
		Parameter( PARAMETER_TAG, const TQuantity & Quantity ) noexcept( false );

		/**
		 * @brief Update signal connection
		 *
		 * @param [in] Slot		Method to call once the signal is emitted
		 */
		virtual void connectUpdateSignal( const UpdateSignalType::slot_type & Slot ) final
		{
			mUpdateSignalConnection = mUpdateSignal->connect( Slot );
		}

		/**
		 * @brief Parameter property flag: isLimited
		 *
		 * @returns true	{Parameter has some valid limits set but not default ones}
		 * @returns false	{Parameter has invalid (or default) limits set}
		 */
		bool isLimited( void ) const
		{
			/* If MIN value is not the lowest possible value (default MIN) and MAX value is not maximum possible value (default MAX), return true, return false otherwise. */
			return( !( ( (this)->mMin == TQuantity::from_value( std::numeric_limits<TData>::lowest() ) ) && ( (this)->mMax == TQuantity::from_value( std::numeric_limits<TData>::max() ) ) ) );
		}

		/**
		 * @brief Setting default limits
		 *
		 * Unless set by setLimits() method, the limits are set to default values of lowest possible number for MIN and to maximum possible number for MAX. Maximum and minimum
		 * number is specific for TData used.
		 */
		void setDefaultLimits( void )
		{
			/* Set default limits to be TData minimum and maximum numeric limits */
			(this)->setLimits( std::make_pair( TQuantity::from_value( std::numeric_limits<TData>::lowest() ), TQuantity::from_value( std::numeric_limits<TData>::max() ) ) );
		}

		/**
		 * @brief Setting limits
		 *
		 * std::pair holds a pair of values where lower value is understood to be minimum value and higher value is understood to be maximum value.
		 *
		 * @param [in] Limits	std::pair of Quantities (TQuantity) defining the limits.
		 *
		 * @throws <ParameterException::OutOfRangeException>	{Once the MIN and MAX values are the same, there is logically no room for any valid parameter quantity}
		 */
		void setLimits( const std::pair<TQuantity, TQuantity> & Limits );

		/**
		 * @brief Get limits
		 *
		 * @returns std::pair<TQuantity, TQuantity>	{std::pair container containing MIN and MAX values}
		 */
		std::pair<TQuantity, TQuantity> getLimits( void ) const
		{
			return( std::make_pair( mMin, mMax ) );
		}

		/**
		 * @brief Set default value
		 *
		 * The method checks the default value whether it fits to the range defined by <MIN,MAX>. If so, the default value is set, an exception is thrown otherwise.
		 *
		 * @param [in] Default	Default quantity value to be evaluated and set.
		 *
		 * @throws <ParameterException::OutOfRangeException>	{Default value cannot be outside the range defined by <MIN,MAX>}
		 */
		void setDefaultValue(const TQuantity Default)
		{
			/* If default value is higher or equal to MIN value and lower or equal to MAX
			 * value (is in range of <MIN, MAX>), it can be taken as default and set. */
			if( ( mMin <= Default ) && ( Default <= mMax) )
			{
				mDefault = Default;
			}
			else
			{
				BOOST_THROW_EXCEPTION( typename Exception::OutOfRange() << Core::Exception::Message( "Default value cannot be set once it is outside the range of <MIN, MAX>" ) );
			}
		}

		/**
		 * @brief Get default value
		 *
		 * @returns TQuantity	{Returns default quantity value}
		 */
		TQuantity const & getDefaultValue( void ) const
		{
			/* TODO: Throw an exception once no default value is set. Is that even possible? Evaluate parameter construction
			 * data flow... Maybe it would be appropriate to set the default to something like infinity value to indicate it
			 * is not set... */
			return( mDefault );
		}

		virtual void resetToDefault( void ) final
		{
			/* Create temporary parameter object */
			TParameter TemporaryParameter( (this)->mSymbol, (this)->mName, (this)->mDefault );

			/* Set limits according to the original */
			TemporaryParameter.setLimits( std::make_pair( (this)->mMin, (this)->mMax ) );

			std::swap( TemporaryParameter, (*this) );
		}

		void setName(const std::string Name)
		{
			/* It makes no sense to set up an empty name. So check the validity here:
			 * If Name is not empty, set it. Throw exception otherwise */
			( !Name.empty() ) ? ( mName = Name ) : BOOST_THROW_EXCEPTION( Exception::InvalidParameterIdentification() << Core::Exception::Message( "Empty name cannot be set." ) );

			/* TODO: Set generated name instead if no name is set at all? */
		}

		virtual std::string const & getName( void ) const final
		{
			return( mName );
		}

		void setSymbol(const std::string Symbol)
		{
			/* It makes no sense to set up an empty symbol. So check the validity here:
			 * If symbol is not empty then set it. Throw exception otherwise */
			( !Symbol.empty() ) ? ( mSymbol = Symbol ) : BOOST_THROW_EXCEPTION( Exception::InvalidParameterIdentification() << Core::Exception::Message( "Empty symbol cannot be set." ) );
		}

		virtual std::string const & getSymbol( void ) const final
		{
			return( mSymbol );
		}

		friend std::ostream & operator << ( std::ostream& OutputStream, const TParameter & Parameter )
		{
			OutputStream << "[Parameter: ";

			if( !Parameter.getName().empty() )
			{
				OutputStream << "'" << Parameter.getName() << "'";
			}
			/* If the symbol is valid... */
			if( !Parameter.getSymbol().empty() )
			{
				/* ...print out symbol including equal character */
				OutputStream << " " << Parameter.getSymbol() << " = ";
			}

			OutputStream << boost::units::engineering_prefix << ( TQuantity ) Parameter;

			if( Parameter.isLimited() )
			{
				/* Get parameter limits */
				std::pair<TQuantity, TQuantity> Limits( Parameter.getLimits() );

				/* Parameter limits are properly set, print them out */
				OutputStream << ", <" << Limits.first << "," << Limits.second << ">";
			}

			OutputStream << "]";

			return OutputStream;
		}

		/* Assignment operator */
		TParameter & operator = ( const TQuantity & Value );

	private:
		/**
		 * @brief Private parameter constructor
		 *
		 * This parameter takes generated parameter identification (name and symbol). As this feature cannot be set from
		 * outside thus this constructor shall be private.
		 *
		 * @param [in] Identification	Identification data generated from ParameterIdentificationGenerator
		 * @param [in] Quantity			Parameter quantity
		 */
		Parameter( const Core::Detail::ParameterIdentificationGenerator::ParameterIdentification & Identification, const TQuantity & Quantity )
			:	Parameter( Identification.mSymbol, Identification.mName, Quantity )
		{}

		/** @var TQuantity mMin	Parameter limits minimum quantity */
		TQuantity	mMin;
		/** @var TQuantity mMax Parameter limits maximum quantity */
		TQuantity	mMax;
		/** @var TQuantity mMin Parameter default quantity */
		TQuantity	mDefault;
		/** @var TQuantity mMin Parameter name */
		std::string	mName;
		/** @var TQuantity mMin Parameter symbol */
		std::string	mSymbol;
		/** @var boost::signals2::signal mUpdateSignal	Parameter quantity updated signal */
		std::shared_ptr<UpdateSignalType>	mUpdateSignal;
		/** @var boost::signals2::connection mUpdateSignalConnection	mUpdateSignal signal connection */
		boost::signals2::connection			mUpdateSignalConnection;
	};

	template<typename UNIT, typename DATA>
	template<typename PARAMETER_TAG>
	Parameter<UNIT, DATA>::Parameter( const PARAMETER_TAG, const TQuantity & Quantity ) noexcept( false )
		:	/* Initialize quantity first */
			TQuantity::quantity( Quantity ),
			/* Construct update signal */
			mUpdateSignal( std::make_shared<UpdateSignalType>() )
	{
		/* Check whether PARAMETER_TAG is compatible with IParameterTag interface */
		static_assert( std::is_base_of<IParameterTagCollection::IParameterTag, PARAMETER_TAG>::value, "Parameter TAG must be derived from IParameterTag." );

		/* As the data must be read from DesignRules DB, check the DB connection first */
		if( DesignRulesDBConnector::get_mutable_instance().isConnected() )
		{
			try
			{
				/* Create SQL query to read all parameter values from design rules DB */
				SQLite::Statement Query( DesignRulesDBConnector::get_mutable_instance(), "SELECT * FROM Parameters WHERE ID = :ConstrainedParameterID LIMIT 1" );

				/* Bind requested parameter identification */
				/* If Identification tag ID type is convertible to required type... */
				if( true == std::is_convertible<typename PARAMETER_TAG::TIdentifier, int>::value )
				{
					/* Bind the SQL query */
					Query.bind(":ConstrainedParameterID", static_cast<int>( PARAMETER_TAG::ID::value ) );
				}
				else
				{
					/* ... else throw an exception */
					BOOST_THROW_EXCEPTION( typename Exception::InvalidParameterIdentification() << Core::Exception::Message( "Identification tag's ID constant type is not convertible." ) );
				}

				/* Process the result */
				while( Query.executeStep() )
				{
					/* TODO: Perform data type check --> Factory to construct type from TData string */
					/* TODO: Perform quantity type check --> Factory to construct type from TQuantity string*/

					/* Load Default quantity value */
					mDefault = TQuantity::from_value( static_cast<TData>( Query.getColumn( "DefaultQuantity" ).getDouble() ) );
					/* Load limits */
					TQuantity tMin = TQuantity::from_value( static_cast<TData>( Query.getColumn( "MinimumQuantity" ).getDouble() ) );
					/* Temporary maximum quantity definition */
					TQuantity tMax;
					/* If MaximumQuantity is set to test value 'INFINITY'... */
					if( ( Query.getColumn( "MaximumQuantity" ).isText() ) && ( std::string( Query.getColumn( "MaximumQuantity" ).getText() ) == "INFINITY" ) )
					{
						/* ... and TData has built-in infinity value, set it. Otherwise set the range maximum value */
						( std::numeric_limits<TData>::has_infinity ) ? tMax = TQuantity::from_value( std::numeric_limits<TData>::infinity() ) : tMax = TQuantity::from_value( std::numeric_limits<TData>::max() );
					}
					else
					{
						/* Reqular maximum quantity */
						tMax = TQuantity::from_value( static_cast<TData>( Query.getColumn( "MaximumQuantity" ).getDouble() ) );
					}

					/* Set the limits */
					(this)->setLimits( std::make_pair( tMin, tMax ) );
					/* Load parameter name */
					mName = std::string( Query.getColumn( "Name" ).getText() );
					/* Load parameter name */
					mSymbol = std::string( Query.getColumn( "Symbol" ).getText() );
				}
			}
			/* Something went wrong during data loading from DB */
			catch( const SQLite::Exception & Exception )
			{
				/* Temporary stringstream used during error message construction */
				std::stringstream ErrorMessage;

				/* Construct error message */
				ErrorMessage << "Requested constrained parameter data cannot be loaded from DesignRules DB. " << Exception.what() << std::endl;

				/* Throw an exception including the error message */
				BOOST_THROW_EXCEPTION( typename Exception::InvalidParameterIdentification() << Core::Exception::Message( ErrorMessage.str() ) );
			}
		}
		/* Database is not connected */
		else
		{
			/* DesignRules database is not connected. */
			BOOST_THROW_EXCEPTION( DesignRulesDBConnector::Exception::DesignRulesDBNotConnected() << Core::Exception::Message( "Design rules database is not connected. Cannot create constrained parameter." ) );
		}
	}

	template<typename UNIT, typename DATA>
	Parameter<UNIT, DATA> & Parameter<UNIT, DATA>::operator = ( const Parameter<UNIT, DATA>::TQuantity & Quantity )
	{
		/* In order to assign the quantity as parameter value, it must fit to the limits */
		if( ( (this)->mMin <= Quantity ) && ( Quantity <= (this)->mMax ) )
		{
			/* Create temporary parameter object */
			TParameter TemporaryParameter( (this)->mSymbol, (this)->mName, Quantity );

			/* Setup it's default value */
			TemporaryParameter.setDefaultValue( (this)->mDefault );

			/* Set limits */
			TemporaryParameter.setLimits( std::make_pair( (this)->mMin, (this)->mMax ) );

			/* Copy signal and it's connection as well */
			TemporaryParameter.mUpdateSignal = (this)->mUpdateSignal;
			TemporaryParameter.mUpdateSignalConnection = (this)->mUpdateSignalConnection;

			/* Once the program flow goes up here there are no exceptions throws during Parameter copy construction so the temporary parameter
			 * can be swapped with the original one */
			/* Swap the temporary and original parameter so that the result contains
			 * updated value only while keeping all other parameters from the original one */
			std::swap( TemporaryParameter, (*this) );

			/* Emit UpdateSignal */
			if( mUpdateSignalConnection.connected() )
			{
				(* (this)->mUpdateSignal)();
			}
		}
		else
		{
			std::stringstream ErrorMessage;

			ErrorMessage << "Quantity value cannot be assigned to Parameter '" << (this)->mName << "' value because it is out of range. Quantity = " << Quantity << ", Parameter range = <" << (this)->mMin << ", " << (this)->mMax << ">" << std::endl;

			BOOST_THROW_EXCEPTION( typename Exception::OutOfRange() << Core::Exception::Message( ErrorMessage.str().c_str() ) );
		}

		/* Return new original */
		return ( (*this) );
	}

	/* FIXME: If the limits are set after the parameter quantity is set and the quantity is out of new limits... */
	template<typename UNIT, typename DATA>
	void Parameter<UNIT, DATA>::setLimits( const std::pair<TQuantity, TQuantity> & Limits )
	{
		TQuantity Min( Limits.first );
		TQuantity Max( Limits.second );

		/* MIN and MAX are NOT equal --> Continue and possibly swap them.
		 * Vector length must be equal to 2 holding just MIN and MAX.
		 */
		if( Min != Max )
		{
			/* MIN value is higher than MAX value --> Swap it! */
			if ( Min > Max )
			{
				/* Swap the two limits */
				std::swap(Min, Max);
			}

			/* Now the limits are NOT equal and in the right order --> Store them */
			mMin = Min;
			mMax = Max;
		}
		else
		{
			/* If MIN and MAX are equal, there is no room left for any valid parameter value */
			BOOST_THROW_EXCEPTION( typename Exception::OutOfRange() << Core::Exception::Message( "Minimum and Maximum values are the same. There is no room to set any valid parameter value." ) );
		}
	}
}
#endif /* CORE_PARAMETER_PARAMETER_H_ */
