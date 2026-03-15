#pragma once

#include <my_lisp/sexpression.hpp>
#include <my_lisp/tokenizer.hpp>

// Parse a single S-expression from the tokenizer. On EOF returns Nil.
SExpression read_expression(Tokenizer &tokenizer);
