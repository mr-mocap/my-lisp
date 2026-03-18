#pragma once

#include <my_lisp/sexpression.hpp>
#include <my_lisp/parse_error.hpp>
#include <expected>


using ParseResult = std::expected<SExpression, ParseError>;
