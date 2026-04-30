#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <my_lisp/types/symbol.hpp>
#include <my_lisp/types/stream.hpp>
#include <my_lisp/sexpression.hpp>


namespace Concepts
{

template <typename T>
concept NilLike = std::same_as<T, FundamentalType::Nil>;

template <typename T>
concept TrueLike = std::same_as<T, FundamentalType::True>;

template <typename T>
struct IsStringArrayLike
{
    static constexpr bool value = false;
};

template <size_t Extent>
struct IsStringArrayLike<const char8_t (&)[Extent]>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_string_array_like_v = IsStringArrayLike<T>::value;

template <typename T>
concept StringArrayLike = is_string_array_like_v<T>;

template <typename T>
concept StringLike = std::same_as<T, FundamentalType::String> || std::same_as<T, FundamentalType::StringView> ||
                     std::same_as<T, StringArrayLike>;

template <typename T>
concept PathnameLike = std::same_as<T, FundamentalType::Pathname>;

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
concept StreamLike = std::same_as<T, FundamentalType::StreamPtr>;

template <typename T>
concept ConsCellLike = std::same_as<T, FundamentalType::ConsCellPtr>;

template <typename T>
concept BoolLike = std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T>
concept VariantLike = NilLike<T>     || TrueLike<T>        || StringLike<T> || PathnameLike<T> || SymbolLike<T> ||
                      NumberLike<T>  || FixedNumberLike<T> || CharLike<T>   || FunctionLike<T> ||
                      PackageLike<T> || StreamLike<T>      || ConsCellLike<T>;

template <typename T>
concept UniversalVariantLike = VariantLike< std::remove_cvref_t<T> >;

template <typename T>
concept SExpressionLike = std::same_as<T, SExpression>;

template <typename T>
concept UniversalSExpressionLike = std::same_as<std::remove_cvref_t<T>, SExpression>;

}
