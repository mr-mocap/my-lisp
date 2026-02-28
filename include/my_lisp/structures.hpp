#pragma once

#include <my_lisp/lisp_library_export.hpp>

#include <my_lisp/Symbol.hpp>
#include <variant>
#include <memory>

struct ConsCell;

using Nil         = std::monostate;
using ConsCellPtr = std::shared_ptr<ConsCell>;
using String      = std::string;


struct BasicVariantType
{
  std::variant<Nil, ConsCellPtr, String, Symbol> value;
};

struct ConsCell
{
  BasicVariantType first;
  BasicVariantType rest;
};
