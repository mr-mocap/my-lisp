#pragma once

#include <my_lisp/lisp_library_export.hpp>
#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/environment.hpp>
#include <ostream>


ConsCellPtr cons(SExpression first, SExpression rest);

void find_symbol(const Environment &environment, StringView name);

void print(const SExpression &expr, std::ostream &output);
