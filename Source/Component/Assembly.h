/*
 * Assembly.h
 *
 *  Created on: 20. 4. 2017
 *      Author: martin
 */

#ifndef CORE_COMPONENT_ASSEMBLY_H_
#define CORE_COMPONENT_ASSEMBLY_H_

/* Standard library inclusions */
#include <memory>
#include <list>

/* Project specific inclusions */
#include "Component/IComponent.h"

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
	class CORE_NO_EXPORT Assembly
		: /* Assembly is handled as higher level component */
		  public IComponent
	{
	public:
		const std::string & getName( void )
		{
			return( this->mAssemblyName );
		}

		void add( const std::shared_ptr<IComponent> & Component )
		{
			this->mComponents.push_back( Component );
		}


	protected:

	private:
		std::string mAssemblyName;

		std::list<std::shared_ptr<IComponent>>	mComponents;
	};
}

#endif /* CORE_COMPONENT_ASSEMBLY_H_ */
