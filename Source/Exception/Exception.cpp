/*
 * Exception.cpp
 *
 *  Created on: 9. 8. 2016
 *      Author: martin
 */

/* Standard library inclusions */
#include <sstream>
#include <iostream>

/* Boost library inclusions */

/* Project specific inclusions */
#include "Exception/Exception.h"

using namespace Core;

const char * Exception::Generic::what( void ) const throw()
{
	/* TODO: Implement custom diagnostic information
	 * See diagnostic_information.hpp
	 * Use std::ostringstream
	 */
	bool BeVerbose = false;

	return( boost::diagnostic_information_what( (*this), BeVerbose ) );
}
