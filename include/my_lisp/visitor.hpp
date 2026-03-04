#pragma once


/** Helper class for use with std::visit
 *
 *
 *  @tparam Callable Any callable type or sequence of callable types
 *
 *  @code {.cpp}
 *  std::variant<float, int, std::string> value{ 4.2 };
 *
 *  std::visit( Visitor{[](float f) { fmt::print("Float: {}\n", f); },
 *                      [](int i) { fmt::print("Int: {}\n", i); },
 *                      [](std::string_view sv) { fmt::print("SV: {}\n", sv); }
 *                     },
 *              value);
 *  @endcode
 *
 *  @note Taken from Jason Turner's example from C++ Weekly.
 */
template <typename ...Callable>
class OverloadedVisitor : Callable...
{
    using operator Callable::operator()...;
};
