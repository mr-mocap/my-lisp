#pragma once

#include <my_lisp/lisp_library_export.hpp>


LISP_LIBRARY_EXPORT
struct Symbol
{
  int value = 0;

  constexpr bool operator==(Symbol other) const noexcept
  {
    return value == other.value; 
  }

  // Let the compiler deduce the != operator (C++20 feature)
};
