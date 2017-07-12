/*
 * IParameterTagCollection.h
 *
 *  Created on: 3. 5. 2017
 *      Author: martin
 */

#ifndef CORE_PARAMETER_IPARAMETERTAGCOLLECTION_H_
#define CORE_PARAMETER_IPARAMETERTAGCOLLECTION_H_

/* Boost inclusions */
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/angle/degrees.hpp>

namespace Core
{
	class IParameterTagCollection
	{
	public:
		class IParameterTag
		{
		public:
			using TIdentifier = int;
		};
	};
}

#endif /* CORE_PARAMETER_IPARAMETERTAGCOLLECTION_H_ */
