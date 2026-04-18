#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <my_lisp/environment.hpp>
#include <my_lisp/contract_helpers.hpp>


namespace PredefinedFunctions
{

SExpression null(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression symbolp(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Symbol )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression atom(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return SExpression::make_nil();
    return SExpression::make_true();
}

SExpression consp(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression listp(Environment &, SExpression parameter)
{
    if ( (parameter.type() == Variant::Type::ConsCell) )
    {
        FundamentalType::ConsCellPtr cons_cell = parameter.asConsCell();

        if ( cons_cell->isList() )
            return SExpression::make_true();
    }
    return SExpression::make_nil();
}

SExpression numberp(Environment &, SExpression parameter)
{
    if ( (parameter.type() == Variant::Type::Number) || (parameter.type() == Variant::Type::FixedNumber) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression integerp(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::FixedNumber )
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
    if ( parameter.type() == Variant::Type::Number )
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
    if ( parameter.type() == Variant::Type::Char )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression stringp(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::String )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression functionp(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Function )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression packagep(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Package )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression eq(Environment &current_environment, SExpression parameter)
{
    // Are these the same, identical object in memory?
    if ( listp(current_environment, parameter) )
    {
        SExpression first_parameter  = first(current_environment, parameter);
        SExpression second_parameter = second(current_environment, parameter);

        if ( first_parameter == second_parameter )
            return SExpression::make_true();
    }
    return SExpression::make_nil();
}

SExpression eql(Environment &current_environment, SExpression parameter)
{
    // Return true if:
    // 1. They are the same object in memory (eq)
    // 2. They are numbers of the same type and value
    // 3. They are characters of the same value

    // 1.
    if ( eq(current_environment, parameter) )
        return SExpression::make_true();

    SExpression first_parameter  = first(current_environment, parameter);
    SExpression second_parameter = second(current_environment, parameter);
    Variant::Type first_type = first_parameter.type();
    Variant::Type second_type = second_parameter.type();

    // They can't be eql if they aren't the same type, so we can return nil immediately in that case.
    if ( first_type != second_type )
        return SExpression::make_nil();

    ASSERT(first_type == second_type, "Since the types are different, they can't be eql, so we should have returned nil already.");

    // 2.
    {
        if ( first_type == Variant::Type::Number )
        {
            if ( first_parameter.asNumber() == second_parameter.asNumber() )
                return SExpression::make_true();
        }

        if ( first_type == Variant::Type::FixedNumber )
        {
            if ( first_parameter.asFixedNumber() == second_parameter.asFixedNumber() )
                return SExpression::make_true();
        }
    }

    // 3.
    if ( first_type == Variant::Type::Char )
    {
        if ( first_parameter.asChar() == second_parameter.asChar() )
            return SExpression::make_true();
    }

    return SExpression::make_nil();
}

SExpression equal(Environment &, SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression equalp(Environment &, SExpression)
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression logical_not(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression pathname(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::String )
        return SExpression{ FundamentalType::Pathname(parameter.asString()) };
    else if ( parameter.type() == Variant::Type::Symbol )
    {
        if ( SExpression *symbol_value = current_environment.find_symbol_value(parameter.asSymbol()) )
        {
            if ( symbol_value->type() == Variant::Type::String )
                return SExpression{ FundamentalType::Pathname(symbol_value->asString()) };
        }
    }
    else if ( parameter.type() == Variant::Type::Pathname )
        return parameter;

    return SExpression::make_nil();
}

//SExpression truename(Environment &current_environment, SExpression parameter);
//SExpression parse_namestring(Environment &current_environment, SExpression parameter);
//SExpression merge_pathnames(Environment &current_environment, SExpression parameter);
//SExpression make_pathname(Environment &current_environment, SExpression parameter);
//SExpression pathnamep(Environment &current_environment, SExpression parameter);
//SExpression pathname_host(Environment &current_environment, SExpression parameter);
//SExpression pathname_device(Environment &current_environment, SExpression parameter);
//SExpression pathname_directory(Environment &current_environment, SExpression parameter);
//SExpression pathname_name(Environment &current_environment, SExpression parameter);
//SExpression pathname_type(Environment &current_environment, SExpression parameter);
//SExpression pathname_version(Environment &current_environment, SExpression parameter);
//SExpression namestring(Environment &current_environment, SExpression parameter);
//SExpression file_namestring(Environment &current_environment, SExpression parameter);
//SExpression directory_namestring(Environment &current_environment, SExpression parameter);
//SExpression host_namestring(Environment &current_environment, SExpression parameter);
//SExpression enough_namestring(Environment &current_environment, SExpression parameter);
//SExpression user_homedir_pathname(Environment &current_environment, SExpression parameter);

SExpression cons(Environment &current_environment, SExpression parameter)
{
    if ( listp(current_environment, parameter) )
    {
        SExpression first_parameter  = first(current_environment, parameter);
        SExpression second_parameter = first( current_environment, rest(current_environment, parameter) );

        return SExpression::make_cons(first_parameter, second_parameter);
    }
    return SExpression::make_nil();
}

SExpression car(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return parameter.asConsCell()->car;
    return SExpression::make_nil();
}

SExpression cdr(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return parameter.asConsCell()->cdr;
    return SExpression::make_nil();
}

SExpression caar(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return car(current_environment, parameter.asConsCell()->car);
    return SExpression::make_nil();
}

SExpression cadr(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return cdr(current_environment, parameter.asConsCell()->car);
    return SExpression::make_nil();
}

SExpression cdar(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return car(current_environment, parameter.asConsCell()->cdr);
    return SExpression::make_nil();
}

SExpression cddr(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return cdr(current_environment, parameter.asConsCell()->cdr);
    return SExpression::make_nil();
}

SExpression first(Environment &current_environment, SExpression parameter)
{
    return car(current_environment, parameter );
}

SExpression rest(Environment &current_environment, SExpression parameter)
{
    if ( listp(current_environment, parameter) )
        return cdr(current_environment, parameter);
    return SExpression::make_nil();
}

SExpression second(Environment &current_environment, SExpression parameter)
{
    return first( current_environment, rest(current_environment, parameter) );
}

SExpression setf(Environment &, SExpression )
{
#if 0
    if ( listp(current_environment, parameter) )
    {
        // TODO: Support pairs of values to set, like (setf a 1 b 2 c 3)
        SExpression first_parameter  = first(current_environment, parameter);
        SExpression second_parameter = first(current_environment, rest(current_environment, parameter) );

        if ( first_parameter.type() == Variant::Type::Symbol )
        {
            if ( SExpression *symbol_value = current_environment.find_symbol_value(first_parameter.asSymbol()) )
            {
                *symbol_value = second_parameter;
                return second_parameter;
            }
        }
    }
#endif
    return SExpression::make_nil();
}

}