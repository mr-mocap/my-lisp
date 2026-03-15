#pragma once

#include <string>
#include <cstddef>

namespace my_lisp
{

struct ParseError
{
    enum Kind
    {
        UnterminatedList,
        MalformedDottedPair,
        UnexpectedEOF,
        UnexpectedToken
    };

    Kind kind;
    std::size_t position{0};
    std::string message;

    ParseError() = default;
    ParseError(Kind k, std::size_t pos, std::string msg)
        : kind(k), position(pos), message(std::move(msg))
    {
    }
};

inline std::string to_string(const ParseError &e)
{
    return e.message + " (pos=" + std::to_string(e.position) + ")";
}

} // namespace my_lisp
