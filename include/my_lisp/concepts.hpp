#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <my_lisp/types/symbol.hpp>


namespace Concepts
{

template <typename T>
concept NilLike = std::same_as<T, FundamentalType::Nil>;

template <typename T>
concept TrueLike = std::same_as<T, FundamentalType::True>;

template <typename T>
concept StringLike = std::same_as<T, FundamentalType::String> || std::same_as<T, FundamentalType::StringView>;

template <typename T>
concept SymbolLike = std::same_as<T, FundamentalType::Symbol>;

template <typename T>
concept NumberLike = std::same_as<T, FundamentalType::Number>;

template <typename T>
concept FixedNumberLike = std::same_as<T, FundamentalType::FixedNumber>;

template <typename T>
concept CharLike = std::same_as<T, FundamentalType::Char>;

template <typename T>
concept FunctionLike = std::same_as<T, FundamentalType::Function>;

template <typename T>
concept PackageLike = std::same_as<T, FundamentalType::PackagePtr>;

template <typename T>
concept ConsCellLike = std::same_as<T, FundamentalType::ConsCellPtr>;

template <typename T>
concept VariantLike = NilLike<T> || TrueLike<T> || StringLike<T> || SymbolLike<T> ||
                      NumberLike<T> || FixedNumberLike<T> || CharLike<T> || FunctionLike<T>||
                      PackageLike<T> || ConsCellLike<T>;
}
