#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <my_lisp/environment.hpp>
#include <my_lisp/contract_helpers.hpp>
#include <my_lisp/reader.hpp>
#include <my_lisp/basic_c_functions.hpp>
#include <print>

// Change this to 1 when I get the checking working correctly
#define CHECK_PARAMETER_COUNT 0

namespace
{

SExpression PredicateFunctionWrapper(const SExpression &parameter, basic_c_functions::PredicateFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 1 )
    {
        // ERROR
        return false;
    }
#endif
    return func( basic_c_functions::first(parameter) );
}

SExpression EqualityFunctionWrapper(const SExpression &parameter, basic_c_functions::EqualityFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 1 )
    {
        // ERROR
        return false;
    }
#endif
    return func( basic_c_functions::first(parameter), basic_c_functions::second(parameter) );
}

SExpression UnaryTransformationFunctionWrapper(const SExpression &parameter, basic_c_functions::UnaryTransformationFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 1 )
    {
        // ERROR
        return false;
    }
#endif
    SExpression first_parameter = basic_c_functions::first(parameter);

    return func( first_parameter );
}

SExpression UnaryTransformationListFunctionWrapper(const SExpression &parameter, basic_c_functions::UnaryTransformationFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 1 )
    {
        // ERROR
        return false;
    }
#endif
    // We already know that parameter is a list, so we can just pass it directly
    return func( parameter );
}

SExpression UnaryTransformationInEnvironmentFunctionWrapper(Environment       &environment,
                                                            const SExpression &parameter,
                                                            basic_c_functions::UnaryTransformationInEnvironmentFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 1 )
    {
        // ERROR
        return false;
    }
#endif
    return func( environment, basic_c_functions::first(parameter) );
}

SExpression BinaryTransformationFunctionWrapper(const SExpression &parameter,
                                                basic_c_functions::BinaryTransformationFunction func)
{
    if ( !basic_c_functions::listp(parameter) )
        return false; // ERROR

#if CHECK_PARAMETER_COUNT
    int length = basic_c_functions::list_length( parameter );

    if ( length != 2 )
    {
        // ERROR
        return false;
    }
#endif
    return func( basic_c_functions::first(parameter), basic_c_functions::second(parameter) );
}

}

namespace PredefinedFunctions
{

SExpression null(Environment &, SExpression parameter)
{
    // Returns true if parameter is an empty list, false (NIL) otherwise.
    return PredicateFunctionWrapper( parameter, basic_c_functions::null );
}

SExpression symbolp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::symbolp );
}

SExpression atom(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::atom );
}

SExpression consp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::consp );
}

SExpression listp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::listp );
}

SExpression numberp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::numberp );
}

SExpression integerp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::integerp );
}

SExpression rationalp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::rationalp );
}

SExpression floatp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::floatp );
}

SExpression realp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::realp );
}

SExpression complexp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::complexp );
}

SExpression characterp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::characterp );
}

SExpression stringp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::stringp );
}

SExpression functionp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::functionp );
}

SExpression packagep(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::packagep );
}

SExpression eq(Environment &, SExpression parameter)
{
    return EqualityFunctionWrapper( parameter, basic_c_functions::eq );
}

SExpression eql(Environment &, SExpression parameter)
{
    return EqualityFunctionWrapper( parameter, basic_c_functions::eql );
}

SExpression equal(Environment &, SExpression parameter)
{
    return EqualityFunctionWrapper( parameter, basic_c_functions::equal );
}

SExpression equalp(Environment &, SExpression parameter)
{
    return EqualityFunctionWrapper( parameter, basic_c_functions::equalp );
}

SExpression logical_not(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::logical_not );
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

SExpression cons(Environment &, SExpression parameter)
{
    return BinaryTransformationFunctionWrapper( parameter, basic_c_functions::cons );
}

SExpression car(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::car );
}

SExpression cdr(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::cdr );
}

SExpression caar(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::caar );
}

SExpression cadr(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::cadr );
}

SExpression cdar(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::cdar );
}

SExpression cddr(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::cddr );
}

SExpression first(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::first );
}

SExpression rest(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::rest );
}

SExpression second(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::second );
}

SExpression third(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::third );
}

SExpression fourth(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::fourth );
}

SExpression fifth(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::fifth );
}

SExpression sixth(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::sixth );
}

SExpression seventh(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::seventh );
}

SExpression eighth(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::eighth );
}

SExpression ninth(Environment &, SExpression parameter)
{
    return UnaryTransformationListFunctionWrapper( parameter, basic_c_functions::ninth );
}

SExpression endp(Environment &, SExpression parameter)
{
    return PredicateFunctionWrapper( parameter, basic_c_functions::endp );
}

//SExpression setf(Environment &, SExpression )
//{
//#if 0
//    if ( listp(current_environment, parameter) )
//    {
//        // TODO: Support pairs of values to set, like (setf a 1 b 2 c 3)
//        SExpression first_parameter  = first(current_environment, parameter);
//        SExpression second_parameter = first(current_environment, rest(current_environment, parameter) );
//
//        if ( first_parameter.type() == Variant::Type::Symbol )
//        {
//            if ( SExpression *symbol_value = current_environment.find_symbol_value(first_parameter.asSymbol()) )
//            {
//                *symbol_value = second_parameter;
//                return second_parameter;
//            }
//        }
//    }
//#endif
//    return SExpression::make_nil();
//}

SExpression prin1(Environment &current_environment, SExpression parameter)
{
    return UnaryTransformationInEnvironmentFunctionWrapper(current_environment,
                                                           parameter,
                                                           basic_c_functions::prin1);
}

SExpression print(Environment &current_environment, SExpression parameter)
{
    return UnaryTransformationInEnvironmentFunctionWrapper(current_environment,
                                                           parameter,
                                                           basic_c_functions::print);
}

SExpression read(Environment &current_environment, SExpression )
{
    Reader input( std::cin );

    ParseResult result = input.read_expression( current_environment );

    if ( !result )
    {
        if ( result.error().kind == ParseError::UnexpectedEOF )
        {
            std::println("EOF");
            return SExpression::make_nil();
        }
        std::println("Parse error: {} at position {}", result.error().message, result.error().position);
        return SExpression::make_nil();
    }
    return result.value();
}

SExpression eval(Environment &current_environment, SExpression form)
{
    return basic_c_functions::eval(current_environment, form);
}

//SExpression eval_list(Environment &current_environment, SExpression parameter)
//{
//#if 0
//(defun eval-list (args)
//    (if (null args)
//        nil
//        (cons (eval (car args))
//            (eval-list(cdr args)))
//)
//#endif
//    if ( null(current_environment, parameter) )
//        return SExpression::make_nil();
//    else
//    {
//        SExpression car_parameter = first(current_environment, parameter);
//        SExpression cdr_parameter = rest(current_environment, parameter);
//
//        return SExpression::make_cons( eval(current_environment, car_parameter),
//                                       eval_list(current_environment, cdr_parameter));
//    }
//}

}