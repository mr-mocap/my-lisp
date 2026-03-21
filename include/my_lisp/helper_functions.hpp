#pragma once

#include <my_lisp/sexpression.hpp>


namespace Helpers
{
constexpr inline SExpression make_nil()
{
    return { .value = ::Nil{} };
}

constexpr inline SExpression make_true()
{
    return { .value = ::True{} };
}

constexpr inline SExpression make_function(FunctionPtr func)
{
    return { .value = func };
}

/**
 *  @note Use "will-move-from" parameters to avoid unnecessary copies.
 *        If the caller has an lvalue, then a copy will be made for the paramter.
 *        If the caller has an rvalue, then the move constructor will be used for the parameter.
 *        Then we just use move internally to move the parameters into the ConsCell, guaranteeing
 *        that we only make one copy/move per argument, regardless of whether the caller has an
 *        lvalue or rvalue.
 */
inline SExpression make_cons(SExpression first, SExpression rest)
{
  return { .value = std::make_shared<ConsCell>( std::move(first), std::move(rest) ) };
} 

}
