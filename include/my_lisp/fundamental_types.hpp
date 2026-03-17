#pragma once

#include <utility>
#include <memory>
#include <string>
#include <variant>

struct ConsCell;

struct TrueValue { };

using Nil         = std::monostate;
using True        = TrueValue;
using ConsCellPtr = std::shared_ptr<ConsCell>;
using String      = std::u8string;

using StringView = std::u8string_view;
