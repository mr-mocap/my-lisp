#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/symbol.hpp>

struct SExpression
{
    // NOTE: The order of the types in this variant is important,
    // as the type() function relies on the index of the variant to determine
    // the type of the SExpression.
    enum Type
    {
        Nil,
        String,
        Symbol,
        Number,
        Boolean,
        Char,
        ConsCell
    };

    std::variant<::Nil, ::String, ::Symbol, double, bool, char32_t, ::ConsCellPtr> value;

    constexpr Type type() const
    {
        return static_cast<enum Type>( value.index() );
    }

    constexpr ::Nil asNil()
    {
        return std::get<::Nil>(value);
    }

    constexpr ::String &asString()
    {
        return std::get<::String>(value);
    }

    constexpr ::Symbol asSymbol()
    {
        return std::get<::Symbol>(value);
    }

    constexpr double asNumber()
    {
        return std::get<double>(value);
    }

    constexpr bool asBoolean()
    {
        return std::get<bool>(value);
    }

    constexpr char32_t asChar()
    {
        return std::get<char32_t>(value);
    }

    ::ConsCellPtr asConsCellPtr()
    {
        return std::get<::ConsCellPtr>(value);
    }
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
        return (car.type() != SExpression::ConsCell && car.type() != SExpression::Nil) &&
                cdr.type() == SExpression::Nil;
    }

    bool isDottedPair() const
    {
        return (car.type() != SExpression::ConsCell) && (cdr.type() != SExpression::ConsCell) &&
               (car.type() != SExpression::Nil) && (cdr.type() != SExpression::Nil);
    }
};
