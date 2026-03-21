#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <my_lisp/helper_functions.hpp>


namespace PredefinedFunctions
{

SExpression null(SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression symbolp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Symbol )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression atom(SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return Helpers::make_nil();
    return Helpers::make_true();
}

SExpression consp(SExpression parameter)
{
    if ( parameter.type() == SExpression::ConsCell )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression listp(SExpression parameter)
{
    if ( (parameter.type() == SExpression::ConsCell) || (parameter.type() == SExpression::Nil) )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression numberp(SExpression parameter)
{
    if ( (parameter.type() == SExpression::Number) || (parameter.type() == SExpression::FixedNumer) )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression integerp(SExpression parameter)
{
    if ( parameter.type() == SExpression::FixedNumer )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression rationalp(SExpression)
{
    // TODO: IMPLEMENT ME
    return Helpers::make_nil();
}

SExpression floatp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Number )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression realp(SExpression)
{
    // TODO: IMPLEMENT ME
    return Helpers::make_nil();
}

SExpression complexp(SExpression)
{
    // TODO: IMPLEMENT ME
    return Helpers::make_nil();
}

SExpression characterp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Char )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression stringp(SExpression parameter)
{
    if ( parameter.type() == SExpression::String )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression functionp(SExpression parameter)
{
    if ( parameter.type() == SExpression::Function )
        return Helpers::make_true();
    return Helpers::make_nil();
}

SExpression logical_not(SExpression parameter)
{
    if ( parameter.type() == SExpression::Nil )
        return Helpers::make_true();
    return Helpers::make_nil();
}

}