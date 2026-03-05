#pragma once

#include <my_lisp/lisp_library_export.hpp>
#include <my_lisp/fundamental_types.hpp>
#include <ostream>


ConsCellPtr cons(SExpression first, SExpression rest);

void print(const SExpression &expr, std::ostream &output);
