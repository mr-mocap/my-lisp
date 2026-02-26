#pragma once


struct Quark
{
  int value = 0;

  constexpr bool operator==(const Quark &other) const noexcept
  {
    return value == other.value; 
  }

  // Let the compiler deduce the != operator (C++20 feature)
};
