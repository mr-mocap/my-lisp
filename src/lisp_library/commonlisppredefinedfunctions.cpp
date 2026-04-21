#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <my_lisp/environment.hpp>
#include <my_lisp/contract_helpers.hpp>

namespace
{

SExpression ExtractParameter(SExpression parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return (*parameter.asConsCellPtr())->car;

    return parameter;
}

bool IsSpecialOperator(SExpression parameter)
{
    if ( parameter.type() == Variant::Type::Symbol )
    {
#if 0
        "block", "catch", "eval-when", "flet", "function", "go", "if", "labels", "let",
        "let*", "load-time-value", "locally", "macrolet", "multiple-value-call", "multiple-value-prog1", "progn", "progv", "quote",
        "return-from", "setq", "symbol-macrolet", "tagbody", "the", "throw", "unwind-protect"
#endif
    }
    return SExpression::make_nil();
}

}

namespace PredefinedFunctions
{

SExpression null(Environment &, SExpression parameter)
{
    // Returns true if parameter is an empty list, false (NIL) otherwise.
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

SExpression listp(Environment &current_environment, SExpression parameter)
{
    if ( consp(current_environment, parameter) )
        return SExpression::make_true();
    return SExpression::make_nil();
}

SExpression numberp(Environment &, SExpression parameter)
{
    Variant::Type parameter_type = parameter.type();

    if ( (parameter_type == Variant::Type::Number) || (parameter_type == Variant::Type::FixedNumber) )
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
    // This compares the structural contents of the objects
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
        SExpression second_parameter = second( current_environment, parameter );

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

SExpression endp(Environment &current_environment, SExpression parameter)
{
    if ( null(current_environment, parameter) )
        return SExpression::make_true();
    if ( cons(current_environment, parameter) )
        return SExpression::make_nil();

    // Error for anything else
    // TODO: Handle the error
    return SExpression::make_nil();
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

SExpression prin1(Environment &current_environment, SExpression parameter)
{
    struct Visitor {
        explicit Visitor(std::ostream &out, const Environment &e) : output(out), env(e) {}
        explicit Visitor(std::ostream &out, const Environment &e, int i) : output(out), env(e), iteration(i) {}

        void operator()(FundamentalType::Nil) const
        {
            std::print(output, "NIL");
        }
        void operator()(FundamentalType::True) const
        {
            std::print(output, "T");
        }
        void operator()(const FundamentalType::String &s) const
        {
            std::print(output, "\"{}\"", text_io::to_string_view(s));
        }
        void operator()(const FundamentalType::Pathname &p) const
        {
            std::print(output, "\"{}\"", text_io::to_string_view(p.u8string()));
        }
        void operator()(FundamentalType::Symbol s) const
        {
            FundamentalType::StringView sym_name( env.symbol_name(s) );

            std::print(output, "{}", (sym_name.empty()) ? "NIL" : text_io::to_string_view(sym_name));
        }
        void operator()(FundamentalType::Number d) const
        {
            std::print(output, "{}", d);
        }
        void operator()(FundamentalType::FixedNumber fixednum) const
        {
            std::print(output, "{}", fixednum);
        }
        void operator()(FundamentalType::Char c) const
        {
            // naive conversion of codepoint to utf-8 bytes is not implemented;
            // print as numeric value for now
            if ( c <= 0x7F )
                std::print(output, "{}", static_cast<char>(c));
            else
                std::print(output, "Printing characters outside of ASCII range not implemented yet");
        }
        void operator()(FundamentalType::Function) const
        {
            std::print(output, "FunctionPtr");
        }
        void operator()(FundamentalType::PackagePtr) const
        {
            std::print(output, "Package");
        }
        void operator()(FundamentalType::ConsCellPtr cons) const
        {
            if ( cons->isEndList() )
            {
                cons->car.visit( Visitor( output, env ) );
                std::print(output, ")");
            }
            else if ( cons->isList() )
            {
                if ( iteration == 0 )
                    std::print(output, "(");

                cons->car.visit( Visitor( output, env ) );
                std::print(output, " ");
                cons->cdr.visit( Visitor( output, env, iteration + 1 ) );
            }
            else if ( cons->isDottedPair() )
            {
                std::print(output, "(");
                cons->car.visit( Visitor( output, env ) );
                std::print(output, " . ");
                cons->cdr.visit( Visitor( output, env ) );
                std::print(output, ")");
            }
        }

        std::ostream &output;
        const Environment &env;
        int iteration = 0;
    };

    parameter.visit( Visitor( std::cout, current_environment ) );
    return parameter;
}

SExpression print(Environment &current_environment, SExpression parameter)
{
    std::print( std::cout, "\n" );
    prin1( current_environment, parameter );
    std::print( std::cout, " " );
    return parameter;
}

SExpression eval(Environment &current_environment, SExpression form)
{
    if ( atom(current_environment, form) )
    {
        // Everything BUT a Symbol is self-evaluating...
        if ( form.selfEvaluating() )
            return form;

        ASSERT(form.type() == Variant::Type::Symbol, "Assert Failed: form is not a Symbol");

        if ( SExpression *value = current_environment.find_symbol_value(*form.asSymbolPtr()) )
            return *value;

        // ERROR: Symbol not found in environment, return nil for now (TODO: Handle this better, maybe throw an error or something)

        return SExpression::make_nil();
    }

    ASSERT( listp(current_environment, form), "Assert Failed: form is not a list");

    // Is this a special form?

    // eval() the list element by element
    SExpression first_element = first(current_environment, form);

    // Let's go ahead and just resolve the first element.  CommonLisp spec requires that the first element NOT
    // be evaluated if it's a symbol that names a special form or macro, but we can handle that later
    // when we implement special forms and macros.  For now, we'll just resolve the first element if it's
    // a symbol, and then we'll handle function application if it turns out to be a function.
    if ( first_element.type() == Variant::Type::Symbol )
    {
        if (SExpression *value = current_environment.find_symbol_value( *first_element.asSymbolPtr() ) )
            (*form.asConsCellPtr())->car = first_element = *value;
    }

    // Handle function application if the first element evaluates to a function
    if ( first_element.type() == Variant::Type::Function )
    {
        // First, evaluate the remaining elements of the list from left to right...
        SExpression rest_of_elements = rest(current_environment, form);

        rest_of_elements = eval(current_environment, rest_of_elements);

        // Call the function with the arguments...
        return first_element.asFunction()(current_environment, rest_of_elements);
    }

    // Macro?
    // TODO

    // Special form?
    // TODO

    return form;
}

}