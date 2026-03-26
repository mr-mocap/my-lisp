#include <my_lisp/commonlisppredefinedfunctions.hpp>


namespace PredefinedFunctions
{

SExpression null(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression symbolp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Symbol )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression atom(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return SExpression::make_nil();
    return SExpression::make_true();
}

SExpression consp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression listp(Environment &, SExpression parameter)
{
    if ( (parameter.type() == SExpression::ConsCell) || (parameter.type() == SExpression::Nil) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression numberp(Environment &, SExpression parameter)
{
    if ( (parameter.type() == SExpression::Number) || (parameter.type() == SExpression::FixedNumber) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression integerp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::FixedNumber )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression rationalp(Environment &, SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression floatp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Number )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression realp(Environment &, SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression complexp(Environment &, SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression characterp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Char )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression stringp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::String )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression functionp(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Function )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression logical_not(Environment &, SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

}