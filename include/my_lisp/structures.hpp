#pragma once

#include <variant>
#include <memory>

struct ConsCell;

using NilType = std::monostate;

using ConsCellPtr = std::shared_ptr<ConsCell>;

struct BasicVariantType
{
  std::variant<NilType, ConsCellPtr> value;
};

struct ConsCell
{
  BasicVariantType first;
  BasicVariantType rest;
};
