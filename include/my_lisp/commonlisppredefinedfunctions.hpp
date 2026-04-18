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

/** @addtogroup PathnameOperations Functions that operate on pathnames
 *  
 *  @{
 *
 *  The following functions use pathnames in some way.
 */
SExpression pathname(Environment &current_environment, SExpression parameter);
SExpression truename(Environment &current_environment, SExpression parameter);
SExpression parse_namestring(Environment &current_environment, SExpression parameter);
SExpression merge_pathnames(Environment &current_environment, SExpression parameter);
SExpression make_pathname(Environment &current_environment, SExpression parameter);
SExpression pathnamep(Environment &current_environment, SExpression parameter);
SExpression pathname_host(Environment &current_environment, SExpression parameter);
SExpression pathname_device(Environment &current_environment, SExpression parameter);
SExpression pathname_directory(Environment &current_environment, SExpression parameter);
SExpression pathname_name(Environment &current_environment, SExpression parameter);
SExpression pathname_type(Environment &current_environment, SExpression parameter);
SExpression pathname_version(Environment &current_environment, SExpression parameter);
SExpression namestring(Environment &current_environment, SExpression parameter);
SExpression file_namestring(Environment &current_environment, SExpression parameter);
SExpression directory_namestring(Environment &current_environment, SExpression parameter);
SExpression host_namestring(Environment &current_environment, SExpression parameter);
SExpression enough_namestring(Environment &current_environment, SExpression parameter);
SExpression user_homedir_pathname(Environment &current_environment, SExpression parameter);
///@}

/** @addtogroup StreamOperations Functions that operate on streams
 *  
 *  @{
 *
 *  The following functions use streams in some way.
 */
SExpression streamp(Environment &current_environment, SExpression parameter);
SExpression open_stream_p(Environment &current_environment, SExpression parameter);
SExpression input_stream_p(Environment &current_environment, SExpression parameter);
SExpression output_stream_p(Environment &current_environment, SExpression parameter);
///@}

/** @addtogroup ConsOperations Functions that operate on cons cells
 *  
 *  @{
 *
 *  The following functions use cons cells in some way.
 */
SExpression cons(Environment &current_environment, SExpression parameter);
SExpression car(Environment &current_environment, SExpression parameter);
SExpression cdr(Environment &current_environment, SExpression parameter);
SExpression caar(Environment &current_environment, SExpression parameter);
SExpression cadr(Environment &current_environment, SExpression parameter);
SExpression cdar(Environment &current_environment, SExpression parameter);
SExpression cddr(Environment &current_environment, SExpression parameter);
///@}

/** @addtogroup ListOperations Functions that operate on lists
 *  
 *  @{
 *
 *  The following functions use lists in some way.
 */
SExpression first(Environment &current_environment, SExpression parameter);
SExpression rest(Environment &current_environment, SExpression parameter);

SExpression second(Environment &current_environment, SExpression parameter);
///@}

SExpression setf(Environment &current_environment, SExpression parameter);

}
