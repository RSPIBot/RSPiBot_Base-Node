/**************************************************************************************\
						templateFunctions.h

Author		:	R.J.H.M.	Stevens
Begin		: 	30-10-2014	
				
Version		:	1.0.0
Changes		:
				30-10-2014	 -			

\**************************************************************************************/
#ifndef TEMPLATEFUNCTIONS_H_
#define TEMPLATEFUNCTIONS_H_

/**************************************************************************************\
                                       Defines
\**************************************************************************************/

/**************************************************************************************\
                                       Includes
\**************************************************************************************/
/* includes for the toString */
#include <sstream>
#include <string>

/**************************************************************************************\
                                 Function templates
\**************************************************************************************/

/**
 * Convert annything to a strng
 *
 * @param value the value that has to be converted to a string
 */
template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif