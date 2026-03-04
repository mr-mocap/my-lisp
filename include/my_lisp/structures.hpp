#pragma once

#include <my_lisp/lisp_library_export.hpp>
#include <my_lisp/fundamental_types.hpp>


ConsCellPtr cons(SExpression first, SExpression rest);

void print(const SExpression &expr);
