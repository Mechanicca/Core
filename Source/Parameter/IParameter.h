/*
 * IParameter.h
 *
 *  Created on: 27. 4. 2017
 *      Author: martin
 */

#ifndef CORE_PARAMETER_IPARAMETER_H_
#define CORE_PARAMETER_IPARAMETER_H_

/* Standard library inclusions */
#include <string>

/* Boost inclusions */
#include <boost/signals2.hpp>

namespace Core
{
	class IParameter
	{
	public:
		using UpdateSignalType = boost::signals2::signal<void ( void )>;

		virtual void connectUpdateSignal( const UpdateSignalType::slot_type & Slot ) = 0;

		virtual void resetToDefault( void ) = 0;

		virtual std::string const & getName( void ) const = 0;

		virtual std::string const & getSymbol( void ) const = 0;

	protected:
		virtual ~IParameter( void ) = default;

	private:

	};
}

#endif /* CORE_PARAMETER_IPARAMETER_H_ */
