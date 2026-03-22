#include <my_lisp/commonlisppredefinedfunctions.hpp>


namespace PredefinedFunctions
{

SExpression null(SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression symbolp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Symbol )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression atom(SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return SExpression::make_nil();
    return SExpression::make_true();
}

SExpression consp(SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression listp(SExpression parameter)
{
    if ( (parameter.type() == SExpression::ConsCell) || (parameter.type() == SExpression::Nil) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression numberp(SExpression parameter)
{
    if ( (parameter.type() == SExpression::Number) || (parameter.type() == SExpression::FixedNumber) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression integerp(SExpression parameter)
{
    if ( parameter.type() == SExpression::FixedNumber )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression rationalp(SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression floatp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Number )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression realp(SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression complexp(SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression characterp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Char )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression stringp(SExpression parameter)
{
    if ( parameter.type() == SExpression::String )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression functionp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Function )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression logical_not(SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

}