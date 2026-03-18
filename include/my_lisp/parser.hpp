#pragma once

#include <my_lisp/types/parsing.hpp>

class Tokenizer;
class Environment;

// Parse a single S-expression from the tokenizer. On EOF returns Nil.

// Parse a single S-expression. On success returns the parsed SExpression.
// On error returns a ParseError describing the failure.
ParseResult read_expression(Tokenizer &tokenizer, Environment &environment);
