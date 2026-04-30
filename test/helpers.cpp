#include "helpers.hpp"

SExpression build_list() // Base case for zero arguments
{
    return SExpression::make_nil();
}

SExpression build_list_from_fundamentals()
{
    return SExpression::make_nil();
}
