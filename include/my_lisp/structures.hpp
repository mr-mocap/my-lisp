#pragma once

#include <my_lisp/lisp_library_export.hpp>
#include <my_lisp/fundamental_types.hpp>


ConsCellPtr cons(SExpression &&first, SExpression &&rest);
ConsCellPtr cons(const SExpression &first, const SExpression &rest);
ConsCellPtr cons(const SExpression &first, SExpression &&rest);
ConsCellPtr cons(SExpression &&first, const SExpression &rest);

void print(const SExpression &expr);
