/*
 * Equation.h
 *
 *  Created on: 18. 8. 2016
 *      Author: martin
 */

#ifndef CORE_EQUATION_EQUATION_H_
#define CORE_EQUATION_EQUATION_H_

/* Standard library inclusions */
#include <type_traits>
#include <sstream>
#include <memory>

/* Project specific inclusions */
#include "Parameter/Parameter.h"

#include "Equation/Exception.h"

/* Forward declarations */
namespace Core
{
	class ParameterContainer;
}

/* Equations are being used in component design */
namespace Design
{
	/**
	 * @brief Equation enabler metafunction [SFINAE]
	 *
	 * When used as class/method enabler, the class/method is enabled once DERIVED is derived from BASE class
	 * or DERIVED is the BASE itself (are same). Then, thanks to SFINAE, the class/method is well-former and
	 * thus is valid and taken into build. Once the condition described above is false, the class/method is
	 * ill-formed and thus removed from compilation.
	 */
	template<typename DERIVED, typename BASE>
	using UsableFor = typename std::enable_if<std::is_base_of<BASE, DERIVED>::value || std::is_same<BASE, DERIVED>::value>::type;

	/**
	 * @brief Empty (undefined) equation
	 *
	 * This equation primary template should never be called once all the equations
	 * are defined - the model is complete. Once called - it means some equation is
	 * not defined but required. So appropriate exception is set up and thrown.
	 *
	 * @throws <Core::Exception::EquationNotDefined>	{Equation required but not defined}
	 */
	template<typename INSTANCE, typename PARAMETER_TAG, typename ENABLER = void>
	struct Equation
	{
		/* The purpose of this method is just to throw an exception with detailed information. In normal operation this functor is expected to be overloaded */
		typename PARAMETER_TAG::TParameter::TQuantity operator() ( const std::shared_ptr<Core::ParameterContainer> Parameters )
		{
			/* Required parameter was not found in the map */
			std::stringstream Stream;

			/* Create instance of parameter identification tag */
			PARAMETER_TAG ID;

			/* Construct temporary parameter of requested identification to load it's properties */
			typename PARAMETER_TAG::TParameter tRequestedParameter( ID );

			/* Write the message into the string stream */
			Stream << "Don't know how to calculate parameter " << tRequestedParameter.getName() << "( " << tRequestedParameter.getSymbol() << " )" << ". The equation is not defined.";

			BOOST_THROW_EXCEPTION( Core::Exception::EquationNotDefined() << Core::Exception::Message( Stream.str() ) );

			/* NOTE: Return statement is never reached --> DUMMY return statement */
			return( typename PARAMETER_TAG::TParameter::TQuantity() );
		}
	};
}

#endif /* CORE_EQUATION_EQUATION_H_ */
