#pragma once

#include <utility>
#include <memory>
#include <string>
#include <variant> // For std::monostate

struct ConsCell;
struct SExpression;
class  Environment;
class  Package;

namespace FundamentalType
{

struct TrueValue { };

using Nil         = std::monostate;
using True        = TrueValue;
using String      = std::u8string;
using Number      = double;
using FixedNumber = int64_t;
using Char        = char32_t;
using Function    = SExpression (*)(Environment &, SExpression);
using PackagePtr  = std::shared_ptr<Package>;
using ConsCellPtr = std::shared_ptr<ConsCell>;

using StringView = std::u8string_view;

}

