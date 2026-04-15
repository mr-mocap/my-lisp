#pragma once

#include <my_lisp/lisp_library_export.hpp>
#include <my_lisp/sexpression.hpp>
#include <my_lisp/environment.hpp>
#include <ostream>


FundamentalType::ConsCellPtr cons(SExpression first, SExpression rest);

void find_symbol(const Environment &environment, FundamentalType::StringView name);

void print(const SExpression &expr, const Environment &environment, std::ostream &output);
