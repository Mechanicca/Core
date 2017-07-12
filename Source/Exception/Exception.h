/*
 * Exception.h
 *
 *  Created on: 9. 8. 2016
 *      Author: martin
 */

#ifndef CORE_EXCEPTION_EXCEPTION_H_
#define CORE_EXCEPTION_EXCEPTION_H_

/* Standard library inclusions */
#include <stdexcept>
#include <string>

/* Boost library inclusions */
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

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
	namespace Exception
	{
		/* TODO: Define all exception attributes here */
		using ThrownBy = boost::error_info<struct tag_ThrownBy, std::string>;
		using Message = boost::error_info<struct tag_Message, std::string>;
		/* TODO ... */

		class CORE_EXPORT Generic
/* FIXME: virtual inheritance does not work here but do not know why... :o( */
#if 0
			:	/* Exception types should use virtual inheritance when deriving from other exception types.
				 * Using virtual inheritance prevents ambiguity problems in the exception handler */
				virtual boost::exception,
				virtual std::exception
#else
			:	public boost::exception,
				public std::exception
#endif
		{
		public:

			virtual const char * what( void ) const throw();

		private:
		};

	} /* namespace Exception */
} /* namespace Core */

#endif /* CORE_EXCEPTION_EXCEPTION_H_ */
