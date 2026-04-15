#include <my_lisp/evaluator.hpp>
#include <my_lisp/environment.hpp>


SExpression &evaluate(SExpression &expr, Environment &)
{
#if 0
    switch ( expr.type() )
    {
    case SExpression::Type::Symbol:
        {
            OptionalSymbol sym_opt = env.find_symbol( expr.as_symbol() );

            if ( sym_opt )
                expr = sym_opt.value();
            else
                return { env.current_package()->intern(expr.as_symbol()) }; // If symbol is not found in environment, intern it in the current package and return that. This allows for forward references to symbols that haven't been defined yet.
        }
        break;

    //case SExpression::Type_e::Function:
    //    return { };
        break;

    default:
        break;
    }
#endif
    return expr;
}