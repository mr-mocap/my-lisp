#pragma once

#include <my_lisp/sexpression.hpp>
#include <my_lisp/concepts.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>


SExpression build_list(); // Base case for zero arguments

template <Concepts::SExpressionLike FirstArg, Concepts::SExpressionLike... RestArgs>
SExpression build_list(FirstArg first, RestArgs ... rest)
{
    SExpression first_node = SExpression::make_cons( std::move(first), build_list( std::move(rest)... ) );

    return first_node;
}
