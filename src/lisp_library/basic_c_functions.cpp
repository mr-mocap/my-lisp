#include <my_lisp/basic_c_functions.hpp>
#include <my_lisp/environment.hpp>
#include <my_lisp/contract_helpers.hpp>
#include <algorithm>
#include <array>
#include <ostream>
#include <print>

namespace basic_c_functions
{

struct SExpressionPrinter {
    explicit SExpressionPrinter(std::ostream &out, Environment &e, const SExpression &p)
        :
        output(out),
        env(e),
        parameter(p)
    {
    }

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
            std::print(output, "\'{}\'", static_cast<char>(c));
        else
            std::print(output, "Printing characters outside of ASCII range not implemented yet");
    }
    void operator()(FundamentalType::Function) const
    {
        std::print(output, "#<Function>");
    }
    void operator()(FundamentalType::PackagePtr) const
    {
        std::print(output, "#<Package>");
    }
    void operator()(FundamentalType::StreamPtr) const
    {
        std::print(output, "#<Stream>");
    }
    void operator()(FundamentalType::ConsCellPtr ) const
    {
        std::print("(");
        print_contents( env, parameter );
        std::print(")");
    }

    std::ostream &output;
    Environment &env;
    const SExpression &parameter;
};

static const std::array<FundamentalType::StringView, 25> SpecialOperatorNames{
    u8"block",
    u8"catch",
    u8"eval-when",
    u8"flet",
    u8"function",
    u8"go",
    u8"if",
    u8"labels",
    u8"let",
    u8"let*",
    u8"load-time-value",
    u8"locally",
    u8"macrolet",
    u8"multiple-value-call",
    u8"multiple-value-prog1",
    u8"progn",
    u8"progv",
    u8"quote",
    u8"return-from",
    u8"setq",
    u8"symbol-macrolet",
    u8"tagbody",
    u8"the",
    u8"throw",
    u8"unwind-protect"
};

bool eql(const SExpression &left, const SExpression &right)
{
    // Return true if:
    // 1. They are the same object in memory (eq)
    // 2. They are numbers of the same type and value
    // 3. They are characters of the same value

    // 1.
    if ( eq(left, right) )
        return true;

    Variant::Type left_type  = left.type();
    Variant::Type right_type = right.type();

    // They can't be eql if they aren't the same type, so we can return false immediately in that case.
    if ( left_type != right_type )
        return false;

    // 2.
    {
        if ( left_type == Variant::Type::Number )
            return *left.asNumberPtr() == *right.asNumberPtr();

        if ( left_type == Variant::Type::FixedNumber )
            return *left.asFixedNumberPtr() == *right.asFixedNumberPtr();
    }

    // 3.
    if ( left_type == Variant::Type::Char )
        return *left.asCharPtr() == *right.asCharPtr();

    return false;
}

bool equal(const SExpression &, const SExpression &)
{
    // This compares the structural contents of the objects
    return false; // TODO: Implement Me!
}

bool equalp(const SExpression &, const SExpression &)
{
    return false; // TODO: Implement Me!
}

SExpression prin1(Environment &current_environment, const SExpression &parameter)
{
    parameter.visit( SExpressionPrinter( std::cout, current_environment, parameter ) );
    return parameter;
}

SExpression print(Environment &current_environment, const SExpression &parameter)
{
    std::print( std::cout, "\n" );
    prin1( current_environment, parameter );
    std::print( std::cout, " " );
    return parameter;
}

void print_contents(Environment &current_environment, const SExpression &parameter)
{
    if ( listp(parameter) )
    {
        if ( endp(parameter) )
            return;

        prin1( current_environment, car(parameter) );

        // Print a separator space if there are more items to print in the list
        if ( consp( rest(parameter) ) )
            std::print(std::cout, " ");

        print_contents( current_environment, rest(parameter) );
    }
    else
        prin1( current_environment, parameter );
}

SExpression eval(Environment &current_environment, const SExpression &form)
{
    if ( atom(form) )
    {
        if ( self_evaluating(form) )
            return form;

        ASSERT(form.type() == Variant::Type::Symbol, "Assert Failed: form is not a Symbol");

        if ( SExpression *value = current_environment.find_symbol_value( *form.asSymbolPtr() ) )
            return *value;

        // ERROR: Symbol not found in environment, return nil for now (TODO: Handle this better, maybe throw an error or something)
        return nil();
    }
    else
    {
        ASSERT( consp(form), "Assert Failed: form is not a cons");

        SExpressionVector forms = list_to_vector( form );
        SExpression &first_element = forms.front();

        // Let's go ahead and just resolve the first element.  CommonLisp spec requires that the first element NOT
        // be evaluated if it's a symbol that names a special form or macro, but we can handle that later
        // when we implement special forms and macros.  For now, we'll just resolve the first element if it's
        // a symbol, and then we'll handle function application if it turns out to be a function.
        if ( first_element.type() == Variant::Type::Symbol )
        {
            // Is this a special form?
            if ( is_special_operator(current_environment, first_element) )
            {
                // TODO: Implement Me!
            }
            else if ( is_macro(current_environment, first_element) )
            {
                // TODO: Implement Macro Expansion
            }

            if (SExpression *value = current_environment.find_symbol_value( *first_element.asSymbolPtr() ) )
                first_element = *value;
        }

        // Handle function application if the first element evaluates to a function
        if ( first_element.type() == Variant::Type::Function )
        {
            // First, evaluate the remaining elements of the list from left to right...
            eval( current_environment, SExpressionSpan(forms).last( forms.size() - 1 ) );

            SExpression rest_of_elements = span_to_list( SExpressionSpan(forms).last( forms.size() - 1 ) );

            // Call the function with the arguments in a lisp-style list...
            SExpression retval = (*first_element.asFunctionPtr())(current_environment, rest_of_elements);

            return cons( retval, nil() );
        }

        return span_to_list( SExpressionSpan(forms) );
    }
}

void eval(Environment &current_environment, SExpressionSpan forms)
{
    std::ranges::transform(forms,
                           forms.begin(),
                           [&current_environment](const SExpression &form)
                           {
                               return eval(current_environment, form);
                           });
}

bool is_special_operator(Environment &current_environment, const SExpression &parameter)
{
    if ( parameter.type() == Variant::Type::Symbol )
    {
        FundamentalType::StringView symbol_name = current_environment.symbol_name( *parameter.asSymbolPtr() );

        if ( symbol_name.empty() )
            return false;

        return std::ranges::contains(SpecialOperatorNames, symbol_name);
    }
    return false;
}

bool is_macro(Environment &, const SExpression &)
{
    return false;
}

int list_length(const SExpression &parameter)
{
    if ( listp(parameter) )
    {
        SExpression fast = parameter;
        SExpression slow = parameter;
        int n = 0;

        while ( true )
        {
            // If fast pointer hits the end, return the count
            if ( endp(fast) )
                return n;

            if ( endp( rest(fast) ) )
                return (n == 0) ? 1 : n;
                //return n + 1;

            // If fast pointer eventually equals slow pointer, then we
            // must be stuck in a circular list.
            // (a deeper property is the converse: if we are stuck in a
            // circular list, then eventually the fast pointer will be equal
            // to the slow pointer.  That fact justifies this implementation)
            // End-of-loop updates
            if ( eq(fast, slow) && (n > 0) )
                return 0; // This is a circular list!
            n += 2;
            fast = second(parameter);
            slow = rest(parameter);
        }
    }

    // ERROR: It is NOT a list
    return -1;
}

std::vector<SExpression> list_to_vector(const SExpression &parameter)
{
    if ( !listp(parameter) )
        return { };

    std::vector<SExpression> items;

#if 0
    items.reserve( list_length(parameter) );
#endif
    for (SExpression iCurrentItem = parameter; !endp(iCurrentItem); iCurrentItem = rest(iCurrentItem))
        items.push_back( first(iCurrentItem) );

    return items;
}

SExpression span_to_list(SExpressionSpan parameter)
{
    if ( parameter.empty() )
        return nil();

    return cons( parameter.front(),
                 span_to_list( parameter.last( parameter.size() - 1 ) ) );
}

}