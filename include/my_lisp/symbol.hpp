#pragma once

#include <my_lisp/fundamental_types.hpp>


struct Symbol
{
  int    value = 0;
  String home_package;

  bool operator==(Symbol other) const noexcept
  {
    return value == other.value;
  }

  // Let the compiler deduce the != operator (C++20 feature)
};
