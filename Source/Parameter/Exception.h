/*
 * Exception.h
 *
 *  Created on: 4. 5. 2017
 *      Author: martin
 */

#ifndef CORE_PARAMETER_EXCEPTION_H_
#define CORE_PARAMETER_EXCEPTION_H_

/* Project specific inclusions */
#include "Exception/Exception.h"

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
		/**
		 * @brief Parameter out of range exception
		 *
		 * ParameterRangeException is thrown if parameter value is detected to be
		 * out of range defined by Parameter's MIN and MAX values. This check is
		 * performed only during the assignment of quantity to parameter. In case
		 * of assignment parameter to parameter, MIN and MAX values are overtaken
		 * as well so no check is performed.
		 */
		struct CORE_EXPORT OutOfRange : virtual Generic {};

		/**
		 * @brief Parameter invalid identification exception
		 *
		 * TODO: Add description when used
		 */
		struct CORE_EXPORT InvalidParameterIdentification : virtual Generic {};

		struct CORE_EXPORT ParameterNotFound : virtual Generic {};
	}
}

#endif /* CORE_PARAMETER_EXCEPTION_H_ */
