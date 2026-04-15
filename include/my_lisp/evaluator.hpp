#pragma once

#include <my_lisp/types/fundamental.hpp>

class Environment;

SExpression evaluate(const SExpression &expr, Environment &env);
