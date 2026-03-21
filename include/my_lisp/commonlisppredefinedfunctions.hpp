#pragma once

#include <my_lisp/sexpression.hpp>


namespace PredefinedFunctions
{

/** @addtogroup DataTypePredicates Specific Data-Type Predicates
 *  
 *  @{
 *
 *  The following predicates check for specific data-types.
 */
SExpression null(SExpression parameter);

SExpression symbolp(SExpression expression);

SExpression atom(SExpression expression);

SExpression consp(SExpression parameter);

SExpression listp(SExpression parameter);

SExpression numberp(SExpression parameter);

SExpression integerp(SExpression parameter);

SExpression rationalp(SExpression parameter);

SExpression floatp(SExpression parameter);

SExpression realp(SExpression parameter);

SExpression complexp(SExpression parameter);

SExpression characterp(SExpression parameter);

SExpression stringp(SExpression parameter);

SExpression functionp(SExpression parameter);

#if 0
SExpression bit-vector-p(SExpression parameter);
SExpression vectorp(SExpression parameter);
SExpression simple-vector-p(SExpression parameter);
SExpression simple-string-p(SExpression parameter);
SExpression somple-bit-vector-p(SExpression parameter);
SExpression arrayp(SExpression parameter);
SExpression packagep(SExpression parameter);
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
//SExpression eq(SExpression left, SExpression right);
//SExpression eql(SExpression left, SExpression right);
//SExpression equal(SExpression left, SExpression right);
//SExpression equalp(SExpression left, SExpression right);
///@}

/** @addtogroup LogicalOperators Various Logical Operations
 *  
 *  @{
 *
 *  The following functions perform logical operations.
 */
SExpression logical_not(SExpression parameter); // "not" as it is in CommonLisp
//SExpression and(SExpression left, SExpression right); // Macro
//SExpression or(SExpression left, SExpression right); // Macro
///@}

}
