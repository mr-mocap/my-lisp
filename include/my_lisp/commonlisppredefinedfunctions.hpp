#pragma once

#include <my_lisp/sexpression.hpp>

class Environment;

namespace PredefinedFunctions
{

/** @addtogroup DataTypePredicates Specific Data-Type Predicates
 *  
 *  @{
 *
 *  The following predicates check for specific data-types.
 */
SExpression null(Environment &current_environment, SExpression parameter);

SExpression symbolp(Environment &current_environment, SExpression expression);

SExpression atom(Environment &current_environment, SExpression expression);

SExpression consp(Environment &current_environment, SExpression parameter);

SExpression listp(Environment &current_environment, SExpression parameter);

SExpression numberp(Environment &current_environment, SExpression parameter);

SExpression integerp(Environment &current_environment, SExpression parameter);

SExpression rationalp(Environment &current_environment, SExpression parameter);

SExpression floatp(Environment &current_environment, SExpression parameter);

SExpression realp(Environment &current_environment, SExpression parameter);

SExpression complexp(Environment &current_environment, SExpression parameter);

SExpression characterp(Environment &current_environment, SExpression parameter);

SExpression stringp(Environment &current_environment, SExpression parameter);

SExpression functionp(Environment &current_environment, SExpression parameter);

SExpression packagep(Environment &, SExpression parameter);

#if 0
SExpression bit-vector-p(SExpression parameter);
SExpression vectorp(SExpression parameter);
SExpression simple-vector-p(SExpression parameter);
SExpression simple-string-p(SExpression parameter);
SExpression somple-bit-vector-p(SExpression parameter);
SExpression arrayp(SExpression parameter);
SExpression compiled-function-p(SExpression parameter);
SExpression commonp(SExpression parameter);
#endif
///@}

/** @addtogroup EqualityPredicates Functions that test for equality
 *  
 *  @{
 *
 *  The following functions check for various forms of equality.
 */
SExpression eq(Environment &current_environment, SExpression parameter);
SExpression eql(Environment &current_environment, SExpression parameter);
SExpression equal(Environment &current_environment, SExpression parameter);
SExpression equalp(Environment &current_environment, SExpression parameter);
///@}

/** @addtogroup LogicalOperators Various Logical Operations
 *  
 *  @{
 *
 *  The following functions perform logical operations.
 */
SExpression logical_not(Environment &current_environment, SExpression parameter); // "not" as it is in CommonLisp
//SExpression and(SExpression left, SExpression right); // Macro
//SExpression or(SExpression left, SExpression right); // Macro
///@}

}
