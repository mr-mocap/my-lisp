#pragma once

#include <my_lisp/sexpression.hpp>
#include <my_lisp/concepts.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>


SExpression build_list() // Base case for zero arguments
{
    return SExpression::make_nil();
}

template <Concepts::SExpressionLike FirstArg, Concepts::SExpressionLike... RestArgs>
SExpression build_list(FirstArg first, RestArgs ... rest)
{
    SExpression first_node = SExpression::make_cons( std::move(first), build_list( std::move(rest)... ) );

    return first_node;
}

#if 0
// one
SExpression result = SExpression::make_cons( std::forward<FirstArg>(first), SExpression::make_nil() )

// two
SExpression result = SExpression::make_cons( std::forward<FirstArg>(first), SExpression::make_cons( std::forward<SecondArg>(second), SExpression::make_nil() ) )

// three
SExpression result = SExpression::make_cons( std::forward<FirstArg>(first),
                                             SExpression::make_cons( std::forward<SecondArg>(second), 
                                                 SExpression::make_cons( std::forward<ThirdArg>(third), SExpression::make_nil() ) ) )

#endif
