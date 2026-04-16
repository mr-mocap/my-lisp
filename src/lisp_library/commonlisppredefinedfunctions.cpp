#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <my_lisp/environment.hpp>


namespace PredefinedFunctions
{

SExpression null(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression symbolp(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::Symbol )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression atom(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::ConsCell )
        return SExpression::make_nil();
    return SExpression::make_true();
}

SExpression consp(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::ConsCell )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression listp(Environment &, SExpression parameter)
{
    if ( (parameter.value().type() == Variant::Type::ConsCell) || (parameter.value().type() == Variant::Type::Nil) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression numberp(Environment &, SExpression parameter)
{
    if ( (parameter.value().type() == Variant::Type::Number) || (parameter.value().type() == Variant::Type::FixedNumber) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression integerp(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::FixedNumber )
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
    if ( parameter.value().type() == Variant::Type::Number )
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
    if ( parameter.value().type() == Variant::Type::Char )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression stringp(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::String )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression functionp(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::Function )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression packagep(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::Package )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression eq(Environment &, SExpression )
{
    // Are these the same, identical object in memory?
    // TODO: IMPLEMENT ME (compare underlying pointers)
    return SExpression::make_nil();
}

SExpression eql(Environment &, SExpression )
{
    // TODO: IMPLEMENT ME
    // Return true if:
    // 1. They are the same object in memory (eq)
    // 2. They are numbers of the same type and value
    // 3. They are characters of the same value
    return SExpression::make_nil();
}

SExpression equal(Environment &, SExpression )
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression equalp(Environment &, SExpression )
{
    // TODO: IMPLEMENT ME
    return SExpression::make_nil();
}

SExpression logical_not(Environment &, SExpression parameter)
{
    if ( parameter.value().type() == Variant::Type::Nil )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression pathname(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::String )
        return SExpression{ FundamentalType::Pathname( parameter.asString() ) };
    else if ( parameter.type() == Variant::Type::Symbol )
    {
        if (SExpression *symbol_value = current_environment.find_symbol_value( parameter.asSymbol() ))
        {
            if ( symbol_value->type() == Variant::Type::String )
                return SExpression{ FundamentalType::Pathname( symbol_value->asString() ) };
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

SExpression car(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return parameter.asConsCellPtr()->car;
    return SExpression::make_nil();
}

SExpression cdr(Environment &, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return parameter.asConsCellPtr()->cdr;
    return SExpression::make_nil();
}

SExpression caar(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return car( current_environment, parameter.asConsCellPtr()->car );
    return SExpression::make_nil();
}

SExpression cadr(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return cdr( current_environment, parameter.asConsCellPtr()->car );
    return SExpression::make_nil();
}

SExpression cdar(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return car( current_environment, parameter.asConsCellPtr()->cdr );
    return SExpression::make_nil();
}

SExpression cddr(Environment &current_environment, SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return cdr( current_environment, parameter.asConsCellPtr()->cdr );
    return SExpression::make_nil();
}

}