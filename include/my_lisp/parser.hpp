#pragma once

#include <my_lisp/sexpression.hpp>
#include <my_lisp/tokenizer.hpp>
#include <my_lisp/parse_error.hpp>
#include <expected>

// Parse a single S-expression from the tokenizer. On EOF returns Nil.

using ParseResult = std::expected<SExpression, my_lisp::ParseError>;

// Parse a single S-expression. On success returns the parsed SExpression.
// On error returns a ParseError describing the failure.
ParseResult read_expression(Tokenizer &tokenizer);
