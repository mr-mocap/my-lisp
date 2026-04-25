#pragma once

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <variant> // For std::monostate
#include <filesystem>

struct ConsCell;
struct SExpression;
class  Environment;
class  Package;
class  Stream;

namespace FundamentalType
{

struct TrueValue { };

using Nil         = std::monostate;
using True        = TrueValue;
using String      = std::u8string;
using Pathname    = std::filesystem::path;
using Number      = double;
using FixedNumber = int64_t;
using Char        = char32_t;
using Function    = SExpression (*)(Environment &, SExpression);
using PackagePtr  = std::shared_ptr<Package>;
using StreamPtr   = std::shared_ptr<Stream>;
using ConsCellPtr = std::shared_ptr<ConsCell>;

using StringView = std::u8string_view;

}

