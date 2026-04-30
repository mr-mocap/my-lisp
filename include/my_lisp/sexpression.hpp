#pragma once

#include <my_lisp/types/variant.hpp>


struct SExpression
{
    constexpr SExpression() noexcept = default;

#if 1
    // Forward type-handling to the underlying Variant constructor
    template <typename T>
#else
    template <Concepts::VariantLike T>
#endif
    SExpression(T &&parameter)
        :
        _value( std::make_shared<Variant>( std::forward<T>(parameter) ) )
    {
    }

    SExpression(SExpression &other)
        :
        _value( other._value )
    {
    }

    SExpression(const SExpression &other)
        :
        _value( other._value )
    {
    }

    SExpression(SExpression &&other) noexcept
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

    constexpr const FundamentalType::Nil *asNilPtr() const
    {
        return _value->asNilPtr();
    }

    constexpr FundamentalType::Nil *asNilPtr()
    {
        return _value->asNilPtr();
    }

    constexpr FundamentalType::True asTrue()
    {
        return _value->asTrue();
    }

    constexpr const FundamentalType::True *asTruePtr() const
    {
        return _value->asTruePtr();
    }

    constexpr FundamentalType::True *asTruePtr()
    {
        return _value->asTruePtr();
    }

    constexpr FundamentalType::String &asString()
    {
        return _value->asString();
    }

    constexpr const FundamentalType::String &asString() const
    {
        return _value->asString();
    }

    constexpr const FundamentalType::String *asStringPtr() const
    {
        return _value->asStringPtr();
    }

    constexpr FundamentalType::String *asStringPtr()
    {
        return _value->asStringPtr();
    }

    constexpr FundamentalType::Pathname &asPathname()
    {
        return _value->asPathname();
    }

    constexpr const FundamentalType::Pathname &asPathname() const
    {
        return _value->asPathname();
    }

    constexpr const FundamentalType::Pathname *asPathnamePtr() const
    {
        return _value->asPathnamePtr();
    }

    constexpr FundamentalType::Pathname *asPathnamePtr()
    {
        return _value->asPathnamePtr();
    }

    constexpr FundamentalType::Symbol &asSymbol()
    {
        return _value->asSymbol();
    }

    constexpr const FundamentalType::Symbol &asSymbol() const
    {
        return _value->asSymbol();
    }

    constexpr const FundamentalType::Symbol *asSymbolPtr() const
    {
        return _value->asSymbolPtr();
    }

    constexpr FundamentalType::Symbol *asSymbolPtr()
    {
        return _value->asSymbolPtr();
    }

    constexpr FundamentalType::Number asNumber()
    {
        return _value->asNumber();
    }

    constexpr const FundamentalType::Number *asNumberPtr() const
    {
        return _value->asNumberPtr();
    }

    constexpr FundamentalType::Number *asNumberPtr()
    {
        return _value->asNumberPtr();
    }

    constexpr FundamentalType::FixedNumber asFixedNumber()
    {
        return _value->asFixedNumber();
    }

    constexpr const FundamentalType::FixedNumber *asFixedNumberPtr() const
    {
        return _value->asFixedNumberPtr();
    }

    constexpr FundamentalType::FixedNumber *asFixedNumberPtr()
    {
        return _value->asFixedNumberPtr();
    }

    constexpr FundamentalType::Char asChar()
    {
        return _value->asChar();
    }

    constexpr const FundamentalType::Char *asCharPtr() const
    {
        return _value->asCharPtr();
    }

    constexpr FundamentalType::Char *asCharPtr()
    {
        return _value->asCharPtr();
    }

    constexpr FundamentalType::Function asFunction()
    {
        return _value->asFunction();
    }

    constexpr const FundamentalType::Function *asFunctionPtr() const
    {
        return _value->asFunctionPtr();
    }

    constexpr FundamentalType::Function *asFunctionPtr()
    {
        return _value->asFunctionPtr();
    }

    FundamentalType::PackagePtr asPackage()
    {
        return _value->asPackage();
    }

    const FundamentalType::PackagePtr asPackage() const
    {
        return _value->asPackage();
    }

    const FundamentalType::PackagePtr *asPackagePtr() const
    {
        return _value->asPackagePtr();
    }

    FundamentalType::PackagePtr *asPackagePtr()
    {
        return _value->asPackagePtr();
    }

    FundamentalType::StreamPtr asStream()
    {
        return _value->asStream();
    }

    const FundamentalType::StreamPtr asStream() const
    {
        return _value->asStream();
    }

    const FundamentalType::StreamPtr *asStreamPtr() const
    {
        return _value->asStreamPtr();
    }

    FundamentalType::StreamPtr *asStreamPtr()
    {
        return _value->asStreamPtr();
    }

    FundamentalType::ConsCellPtr asConsCell()
    {
        return _value->asConsCell();
    }

    const FundamentalType::ConsCellPtr asConsCell() const
    {
        return _value->asConsCell();
    }

    const FundamentalType::ConsCellPtr *asConsCellPtr() const
    {
        return _value->asConsCellPtr();
    }

    FundamentalType::ConsCellPtr *asConsCellPtr()
    {
        return _value->asConsCellPtr();
    }

    bool isList() const;

    bool selfEvaluating() const
    {
        return type() != Variant::Type::Symbol && !isList();
    }

    void visit(auto &&visitor) const
    {
        _value->visit(visitor);
    }

    constexpr operator bool()
    {
        // Since only nil is false in Lisp, we can return true for all other types, including true.
        return type() != Variant::Type::Nil;
    }
protected:
    std::shared_ptr<Variant> _value{ std::make_shared<Variant>( FundamentalType::Nil{} ) };

    // This compares the two values for being the same underlying object (i.e. they point to the same memory location).
    friend bool operator ==(const SExpression &lhs, const SExpression &rhs)
    {
        return lhs._value == rhs._value;
    }
};

struct ConsCell
{
    SExpression car;
    SExpression cdr;

    bool isList() const
    {
        return (cdr.type() == Variant::Type::ConsCell) || isEndList();
    }

    bool isEndList() const
    {
        return cdr.type() == Variant::Type::Nil;
    }

    bool isDottedPair() const
    {
        return (car.type() != Variant::Type::ConsCell) &&
               (cdr.type() != Variant::Type::ConsCell);
    }
};

inline bool SExpression::isList() const
{
    return type() == Variant::Type::ConsCell && asConsCell()->isList();
}
