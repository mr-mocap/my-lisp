#pragma once


struct Symbol
{
  int value = 0;

  constexpr bool operator==(Symbol other) const noexcept
  {
    return value == other.value; 
  }

  // Let the compiler deduce the != operator (C++20 feature)
};
