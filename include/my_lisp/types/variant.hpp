#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <my_lisp/types/symbol.hpp>
#include <my_lisp/types/stream.hpp>
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
                                    FundamentalType::ConsCellPtr,
                                    FundamentalType::StreamPtr
    >;

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
        ConsCell,
        Stream,
    };

    constexpr Variant() noexcept = default;
    constexpr Variant(const Variant &other) = default;
    constexpr Variant(Variant &&other) noexcept = default;

    constexpr Variant(bool truth) noexcept
        :
        Variant( truth ? Variant{ FundamentalType::True{} } : Variant{ FundamentalType::Nil{} } )
    {
    }

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

    // Initialize with something like Variant( u8"Some String" )
    template <std::size_t ArrayExtent>
    constexpr Variant(const char8_t (&arr)[ArrayExtent])
        :
        _value(arr)
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

    constexpr Variant &operator =(bool parameter)
    {
        if ( parameter )
            _value = FundamentalType::True{};
        else
            _value = FundamentalType::Nil{};
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

    constexpr std::size_t index() const
    {
        return _value.index();
    }

    constexpr bool valueless_by_exception() const noexcept
    {
        return _value.valueless_by_exception();
    }

    constexpr FundamentalType::Nil asNil()
    {
        return std::get<FundamentalType::Nil>(_value);
    }

    constexpr const FundamentalType::Nil *asNilPtr() const
    {
        return std::get_if<FundamentalType::Nil>(&_value);
    }

    constexpr FundamentalType::Nil *asNilPtr()
    {
        return std::get_if<FundamentalType::Nil>(&_value);
    }

    constexpr FundamentalType::True asTrue()
    {
        return std::get<FundamentalType::True>(_value);
    }

    constexpr const FundamentalType::True *asTruePtr() const
    {
        return std::get_if<FundamentalType::True>(&_value);
    }

    constexpr FundamentalType::True *asTruePtr()
    {
        return std::get_if<FundamentalType::True>(&_value);
    }

    constexpr FundamentalType::String &asString()
    {
        return std::get<FundamentalType::String>(_value);
    }

    constexpr const FundamentalType::String &asString() const
    {
        return std::get<FundamentalType::String>(_value);
    }

    constexpr const FundamentalType::String *asStringPtr() const
    {
        return std::get_if<FundamentalType::String>(&_value);
    }

    constexpr FundamentalType::String *asStringPtr()
    {
        return std::get_if<FundamentalType::String>(&_value);
    }

    constexpr FundamentalType::Pathname &asPathname()
    {
        return std::get<FundamentalType::Pathname>(_value);
    }

    constexpr const FundamentalType::Pathname &asPathname() const
    {
        return std::get<FundamentalType::Pathname>(_value);
    }

    constexpr FundamentalType::Pathname *asPathnamePtr()
    {
        return std::get_if<FundamentalType::Pathname>(&_value);
    }

    constexpr const FundamentalType::Pathname *asPathnamePtr() const
    {
        return std::get_if<FundamentalType::Pathname>(&_value);
    }

    constexpr FundamentalType::Symbol &asSymbol()
    {
        return std::get<FundamentalType::Symbol>(_value);
    }

    constexpr const FundamentalType::Symbol &asSymbol() const
    {
        return std::get<FundamentalType::Symbol>(_value);
    }

    constexpr FundamentalType::Symbol *asSymbolPtr()
    {
        return std::get_if<FundamentalType::Symbol>(&_value);
    }

    constexpr const FundamentalType::Symbol *asSymbolPtr() const
    {
        return std::get_if<FundamentalType::Symbol>(&_value);
    }

    constexpr FundamentalType::Number asNumber()
    {
        return std::get<FundamentalType::Number>(_value);
    }

    constexpr const FundamentalType::Number *asNumberPtr() const
    {
        return std::get_if<FundamentalType::Number>(&_value);
    }

    constexpr FundamentalType::Number *asNumberPtr()
    {
        return std::get_if<FundamentalType::Number>(&_value);
    }

    constexpr FundamentalType::FixedNumber asFixedNumber()
    {
        return std::get<FundamentalType::FixedNumber>(_value);
    }

    constexpr const FundamentalType::FixedNumber *asFixedNumberPtr() const
    {
        return std::get_if<FundamentalType::FixedNumber>(&_value);
    }

    constexpr FundamentalType::FixedNumber *asFixedNumberPtr()
    {
        return std::get_if<FundamentalType::FixedNumber>(&_value);
    }

    constexpr FundamentalType::Char asChar()
    {
        return std::get<FundamentalType::Char>(_value);
    }

    constexpr const FundamentalType::Char *asCharPtr() const
    {
        return std::get_if<FundamentalType::Char>(&_value);
    }

    constexpr FundamentalType::Char *asCharPtr()
    {
        return std::get_if<FundamentalType::Char>(&_value);
    }

    constexpr FundamentalType::Function asFunction()
    {
        return std::get<FundamentalType::Function>(_value);
    }

    constexpr const FundamentalType::Function *asFunctionPtr() const
    {
        return std::get_if<FundamentalType::Function>(&_value);
    }

    constexpr FundamentalType::Function *asFunctionPtr()
    {
        return std::get_if<FundamentalType::Function>(&_value);
    }

    FundamentalType::PackagePtr asPackage()
    {
        return std::get<FundamentalType::PackagePtr>(_value);
    }

    const FundamentalType::PackagePtr asPackage() const
    {
        return std::get<FundamentalType::PackagePtr>(_value);
    }

    const FundamentalType::PackagePtr *asPackagePtr() const
    {
        return std::get_if<FundamentalType::PackagePtr>(&_value);
    }

    FundamentalType::PackagePtr *asPackagePtr()
    {
        return std::get_if<FundamentalType::PackagePtr>(&_value);
    }

    FundamentalType::ConsCellPtr asConsCell()
    {
        return std::get<FundamentalType::ConsCellPtr>(_value);
    }

    const FundamentalType::ConsCellPtr asConsCell() const
    {
        return std::get<FundamentalType::ConsCellPtr>(_value);
    }

    FundamentalType::ConsCellPtr *asConsCellPtr()
    {
        return std::get_if<FundamentalType::ConsCellPtr>(&_value);
    }

    const FundamentalType::ConsCellPtr *asConsCellPtr() const
    {
        return std::get_if<FundamentalType::ConsCellPtr>(&_value);
    }

    FundamentalType::StreamPtr asStream()
    {
        return std::get<FundamentalType::StreamPtr>(_value);
    }

    const FundamentalType::StreamPtr asStream() const
    {
        return std::get<FundamentalType::StreamPtr>(_value);
    }

    const FundamentalType::StreamPtr *asStreamPtr() const
    {
        return std::get_if<FundamentalType::StreamPtr>(&_value);
    }

    FundamentalType::StreamPtr *asStreamPtr()
    {
        return std::get_if<FundamentalType::StreamPtr>(&_value);
    }

    constexpr const value_type &rawValue() const
    {
        return _value;
    }

    constexpr operator bool() const
    {
        return type() != Nil;
    }

    void visit(auto &&visitor) const
    {
        std::visit( visitor, _value );
    }
protected:
    value_type _value;
};
