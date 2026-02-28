#include <my_lisp/Checks.hpp>
#include <cctype>


bool IsSymbolChar(char input)
{
    return std::isalnum(input);
}

bool IsSymbolChar(char8_t input)
{
    return std::isalnum(input);
}