#pragma once

#include <my_lisp/types/variant.hpp>


struct SExpression
{
    constexpr SExpression() noexcept = default;

    template <Concepts::VariantLike T>
    SExpression(T parameter)
        :
        _value( std::make_shared<Variant>( std::forward<T>(parameter) ) )
    {
    }

    SExpression(FundamentalType::StringView parameter)
        :
        _value( std::make_shared<Variant>( FundamentalType::String(parameter) ) )
    {
    }

    SExpression(const Variant &variant)
        :
        _value( std::make_shared<Variant>( variant ) )
    {
    }

    SExpression(Variant &&variant)
        :
        _value( std::make_shared<Variant>( std::move(variant) ) )
    {
    }

    SExpression(const SExpression &other)
        :
        _value( std::make_shared<Variant>( *other._value ) )
    {
    }

    SExpression(SExpression &&other)
        :
        _value( std::move(other._value) )
    {
    }

    static SExpression make_nil()  { return FundamentalType::Nil{}; }
    static SExpression make_true() { return FundamentalType::True{}; }
    static SExpression make_function(FundamentalType::Function func)
    {
        return { std::move(func) };
    }

    /**
     *  @note Use "will-move-from" parameters to avoid unnecessary copies.
     *        If the caller has an lvalue, then a copy will be made for the paramter.
     *        If the caller has an rvalue, then the move constructor will be used for the parameter.
     *        Then we just use move internally to move the parameters into the ConsCell, guaranteeing
     *        that we only make one copy/move per argument, regardless of whether the caller has an
     *        lvalue or rvalue.
     */
    static SExpression make_cons(SExpression first, SExpression rest)
    {
      return SExpression{ std::make_shared<::ConsCell>( std::move(first), std::move(rest) ) };
    } 

    SExpression &operator =(const SExpression &other)
    {
        if ( &other != this )
            _value = other._value;
        return *this;
    }

    SExpression &operator =(SExpression &&other) noexcept
    {
        if ( &other != this )
            _value = std::move(other._value);
        return *this;
    }

    template <Concepts::VariantLike T>
    SExpression &operator =(T parameter)
    {
        // Reset the value to a new one
        _value = std::make_shared<Variant>( std::forward<T>(parameter) );
        return *this;
    }

    SExpression &operator =(FundamentalType::StringView parameter)
    {
        // Reset the value to a new one
        _value = std::make_shared<Variant>( FundamentalType::String(parameter) );
        return *this;
    }

    constexpr Variant::Type type() const
    {
        return _value->type();
    }

    constexpr FundamentalType::Nil asNil()
    {
        return _value->asNil();
    }

    constexpr FundamentalType::True asTrue()
    {
        return _value->asTrue();
    }

    constexpr FundamentalType::String &asString()
    {
        return _value->asString();
    }

    constexpr const FundamentalType::String &asString() const
    {
        return _value->asString();
    }

    constexpr FundamentalType::Symbol &asSymbol()
    {
        return _value->asSymbol();
    }

    constexpr const FundamentalType::Symbol &asSymbol() const
    {
        return _value->asSymbol();
    }

    constexpr FundamentalType::Number asNumber()
    {
        return _value->asNumber();
    }

    constexpr FundamentalType::FixedNumber asFixedNumber()
    {
        return _value->asFixedNumber();
    }

    constexpr FundamentalType::Char asChar()
    {
        return _value->asChar();
    }

    constexpr FundamentalType::Function asFunction()
    {
        return _value->asFunction();
    }

    FundamentalType::PackagePtr asPackage()
    {
        return _value->asPackage();
    }

    const FundamentalType::PackagePtr asPackage() const
    {
        return _value->asPackage();
    }

    FundamentalType::ConsCellPtr asConsCellPtr()
    {
        return _value->asConsCellPtr();
    }

    const FundamentalType::ConsCellPtr asConsCellPtr() const
    {
        return _value->asConsCellPtr();
    }

    Variant &value()
    {
        return *_value;
    }

    const Variant &value() const
    {
        return *_value;
    }

    const Variant::value_type &rawValue() const
    {
        return _value->rawValue();
    }

    bool isList() const;

    bool selfEvaluating() const
    {
        return value().type() != Variant::Type::Symbol && !isList();
    }
protected:
    std::shared_ptr<Variant> _value{ std::make_shared<Variant>( FundamentalType::Nil{} ) };
};

struct ConsCell
{
    SExpression car;
    SExpression cdr;

    bool isList() const
    {
        return cdr.value().type() == Variant::Type::ConsCell;
    }

    bool isEndList() const
    {
        return cdr.value().type() == Variant::Type::Nil;
    }

    bool isDottedPair() const
    {
        return (car.value().type() != Variant::Type::ConsCell) &&
               (cdr.value().type() != Variant::Type::ConsCell);
    }
};

inline bool SExpression::isList() const
{
    return value().type() == Variant::Type::ConsCell && value().asConsCellPtr()->isList();
}
