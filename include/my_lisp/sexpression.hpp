#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/symbol.hpp>

struct SExpression
{
    using VariantType = std::variant<::Nil,
                                     ::True,
                                     ::String,
                                     ::Symbol,
                                     ::Number,
                                     ::FixedNumber,
                                     ::Char,
                                     ::Function,
                                     ::ConsCellPtr>;

    // NOTE: The order of the types in this variant is important,
    // as the type() function relies on the index of the variant to determine
    // the type of the SExpression.
    enum Type
    {
        Nil,
        True,
        String,
        Symbol,
        Number,
        FixedNumber,
        Char,
        Function,
        ConsCell
    };

    constexpr SExpression() : value( ::Nil{} ) {}
    constexpr SExpression(::Nil         parameter) : value( parameter ) { }
    constexpr SExpression(::True        parameter) : value( parameter ) { }
    explicit constexpr SExpression(::String      parameter) : value( std::move(parameter) ) { }
    constexpr SExpression(::StringView  parameter) : value( ::String(parameter) ) { }
    constexpr SExpression(::Symbol      parameter) : value( std::move(parameter) ) { }
    constexpr SExpression(::Number      parameter) : value( parameter ) { }
    constexpr SExpression(::FixedNumber parameter) : value( parameter ) { }
    constexpr SExpression(::Char        parameter) : value( parameter ) { }
    constexpr SExpression(::Function    parameter) : value( std::move(parameter) ) { }
    constexpr SExpression(::ConsCellPtr parameter) : value( std::move(parameter) ) { }

    static constexpr SExpression make_nil()  { return SExpression{}; }
    static constexpr SExpression make_true() { return SExpression{ ::True{} }; }

    static constexpr SExpression make_function(::Function func)
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
      return { std::make_shared<::ConsCell>( std::move(first), std::move(rest) ) };
    } 

    SExpression &operator =(const SExpression &other)
    {
        if ( &other != this )
            value = other.value;
        return *this;
    }

    constexpr SExpression &operator =(::Nil parameter)
    {
        value.emplace<Type::Nil>( std::move(parameter) );
        return *this;
    }

    constexpr SExpression &operator =(::True parameter)
    {
        value.emplace<Type::True>( std::move(parameter) );
        return *this;
    }

    constexpr SExpression &operator =(::StringView parameter)
    {
        value.emplace<Type::String>( parameter );
        return *this;
    }

    constexpr SExpression &operator =(::Symbol parameter)
    {
        value.emplace<Type::Symbol>( std::move(parameter) );
        return *this;
    }

    constexpr SExpression &operator =(::Number parameter)
    {
        value.emplace<Type::Number>( parameter );
        return *this;
    }

    constexpr SExpression &operator =(::FixedNumber parameter)
    {
        value.emplace<Type::FixedNumber>( parameter );
        return *this;
    }

    constexpr SExpression &operator =(::Char parameter)
    {
        value.emplace<Type::Char>( parameter );
        return *this;
    }

    constexpr SExpression &operator =(::Function parameter)
    {
        value.emplace<Type::Function>( parameter );
        return *this;
    }

    constexpr SExpression &operator =(::ConsCellPtr parameter)
    {
        value.emplace<Type::ConsCell>( std::move(parameter) );
        return *this;
    }

    constexpr Type type() const
    {
        return static_cast<enum Type>( value.index() );
    }

    constexpr ::Nil asNil()
    {
        return std::get<::Nil>(value);
    }

    constexpr ::True asTrue()
    {
        return std::get<::True>(value);
    }

    constexpr ::String &asString()
    {
        return std::get<::String>(value);
    }

    constexpr ::Symbol asSymbol()
    {
        return std::get<::Symbol>(value);
    }

    constexpr ::Number asNumber()
    {
        return std::get<::Number>(value);
    }

    constexpr ::FixedNumber asFixedNumber()
    {
        return std::get<::FixedNumber>(value);
    }

    constexpr ::Char asChar()
    {
        return std::get<::Char>(value);
    }

    constexpr ::Function asFunction()
    {
        return std::get<::Function>(value);
    }

    ::ConsCellPtr asConsCellPtr()
    {
        return std::get<::ConsCellPtr>(value);
    }

    VariantType value;
};

struct ConsCell
{
    SExpression car;
    SExpression cdr;

    bool isListSegment() const
    {
        return car.type() != SExpression::ConsCell && car.type() != SExpression::Nil && cdr.type() == SExpression::ConsCell;
    }

    bool isEndListSegment() const
    {
        return car.type() != SExpression::Nil && cdr.type() == SExpression::Nil;
    }

    bool isDottedPair() const
    {
        return (car.type() != SExpression::ConsCell) && (cdr.type() != SExpression::ConsCell) &&
               (car.type() != SExpression::Nil) && (cdr.type() != SExpression::Nil);
    }
};
