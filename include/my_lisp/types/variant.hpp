#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <my_lisp/types/symbol.hpp>
#include <my_lisp/concepts.hpp>


class Variant
{
public:
    using value_type = std::variant<FundamentalType::Nil,
                                    FundamentalType::True,
                                    FundamentalType::String,
                                    FundamentalType::Pathname,
                                    FundamentalType::Symbol,
                                    FundamentalType::Number,
                                    FundamentalType::FixedNumber,
                                    FundamentalType::Char,
                                    FundamentalType::Function,
                                    FundamentalType::PackagePtr,
                                    FundamentalType::ConsCellPtr>;

    // NOTE: The order of the types in this variant is important,
    // as the type() function relies on the index of the variant to determine
    // the type of the SExpression.
    enum Type
    {
        Nil,
        True,
        String,
        Pathname,
        Symbol,
        Number,
        FixedNumber,
        Char,
        Function,
        Package,
        ConsCell
    };

    constexpr Variant() noexcept = default;
    constexpr Variant(const Variant &other) = default;
    constexpr Variant(Variant &&other) noexcept = default;

    // This is a "sink parameter", which allows us to avoid unnecessary copies when constructing Variants.
    template <Concepts::VariantLike T>
    constexpr Variant(T parameter) noexcept
        :
        _value{ std::forward<T>(parameter) }
    {
    }

    constexpr Variant(FundamentalType::StringView parameter) noexcept
        :
        _value( FundamentalType::String(parameter) )
    {
    }

    constexpr Variant &operator =(const Variant &other) = default;
    constexpr Variant &operator =(Variant &&other) noexcept = default;

    template <Concepts::VariantLike T>
    Variant &operator =(T parameter)
    {
        _value = std::forward<T>(parameter);
        return *this;
    }
    
    constexpr Variant &operator =(FundamentalType::StringView parameter)
    {
        _value = FundamentalType::String(parameter);
        return *this;
    }

    constexpr static Variant make_nil()  { return Variant{}; }
    constexpr static Variant make_true() { return Variant{ FundamentalType::True{} }; }
    constexpr static Variant make_function(FundamentalType::Function func)
    {
        return { std::move(func) };
    }

    constexpr Type type() const
    {
        return static_cast<enum Type>( _value.index() );
    }

    constexpr FundamentalType::Nil asNil()
    {
        return std::get<FundamentalType::Nil>(_value);
    }

    constexpr FundamentalType::True asTrue()
    {
        return std::get<FundamentalType::True>(_value);
    }

    constexpr FundamentalType::String &asString()
    {
        return std::get<FundamentalType::String>(_value);
    }

    constexpr const FundamentalType::String &asString() const
    {
        return std::get<FundamentalType::String>(_value);
    }

    constexpr FundamentalType::Pathname &asPathname()
    {
        return std::get<FundamentalType::Pathname>(_value);
    }

    constexpr const FundamentalType::Pathname &asPathname() const
    {
        return std::get<FundamentalType::Pathname>(_value);
    }

    constexpr FundamentalType::Symbol &asSymbol()
    {
        return std::get<FundamentalType::Symbol>(_value);
    }

    constexpr const FundamentalType::Symbol &asSymbol() const
    {
        return std::get<FundamentalType::Symbol>(_value);
    }

    constexpr FundamentalType::Number asNumber()
    {
        return std::get<FundamentalType::Number>(_value);
    }

    constexpr FundamentalType::FixedNumber asFixedNumber()
    {
        return std::get<FundamentalType::FixedNumber>(_value);
    }

    constexpr FundamentalType::Char asChar()
    {
        return std::get<FundamentalType::Char>(_value);
    }

    constexpr FundamentalType::Function asFunction()
    {
        return std::get<FundamentalType::Function>(_value);
    }

    FundamentalType::PackagePtr asPackage()
    {
        return std::get<FundamentalType::PackagePtr>(_value);
    }

    const FundamentalType::PackagePtr asPackage() const
    {
        return std::get<FundamentalType::PackagePtr>(_value);
    }

    FundamentalType::ConsCellPtr asConsCellPtr()
    {
        return std::get<FundamentalType::ConsCellPtr>(_value);
    }

    const FundamentalType::ConsCellPtr asConsCellPtr() const
    {
        return std::get<FundamentalType::ConsCellPtr>(_value);
    }

    constexpr const value_type &rawValue() const
    {
        return _value;
    }
protected:
    value_type _value;
};
