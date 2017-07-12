/*
 * Exception.h
 *
 *  Created on: 4. 5. 2017
 *      Author: martin
 */

#ifndef CORE_EQUATION_EXCEPTION_H_
#define CORE_EQUATION_EXCEPTION_H_

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
		struct CORE_EXPORT EquationNotDefined : virtual Generic {};
	}
}

#endif /* CORE_EQUATION_EXCEPTION_H_ */
