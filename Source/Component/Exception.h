/*
 * Exceptions.h
 *
 *  Created on: 26. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_EXCEPTION_H_
#define CORE_COMPONENT_EXCEPTION_H_

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

namespace Component
{
	namespace Exception
	{
		namespace Factory
		{
			struct CORE_EXPORT PluginLoadingFailed : virtual Core::Exception::Generic {};
			struct CORE_EXPORT ComponentDesignerNotFound : virtual Core::Exception::Generic {};
		}

		namespace Design
		{
			struct CORE_EXPORT SpecificationIncomplete : virtual Core::Exception::Generic {};
			struct CORE_EXPORT ModelConstructionFailed : virtual Core::Exception::Generic {};
		}
	}
}

#endif /* CORE_COMPONENT_EXCEPTION_H_ */
