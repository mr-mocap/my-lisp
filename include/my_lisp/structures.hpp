#pragma once

#include <my_lisp/lisp_library_export.hpp>

#include <my_lisp/symbol.hpp>
#include <string>
#include <variant>
#include <utility>
#include <memory>

struct ConsCell;

using Nil         = std::monostate;
using ConsCellPtr = std::shared_ptr<ConsCell>;
using String      = std::u8string;

using StringView = std::u8string_view;

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
        ConsCell
    };

    std::variant<::Nil, ::String, ::Symbol, ::ConsCellPtr> value;

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

    ::ConsCellPtr asConsCellPtr()
    {
        return std::get<::ConsCellPtr>(value);
    }
};

struct ConsCell
{
    SExpression car;
    SExpression cdr;
};

ConsCellPtr cons(SExpression &&first, SExpression &&rest);
ConsCellPtr cons(const SExpression &first, const SExpression &rest);
ConsCellPtr cons(const SExpression &first, SExpression &&rest);
ConsCellPtr cons(SExpression &&first, const SExpression &rest);

void print(const SExpression &expr);
