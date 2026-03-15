#include <my_lisp/parser.hpp>
#include <my_lisp/structures.hpp>
#include <my_lisp/symboltable.hpp>
#include <my_lisp/text_io.hpp>
#include <functional>

// Minimal parser that consumes tokens and builds SExpression values.
// - Symbols are interned via a global SymbolTable instance (simple).
// - Numbers and booleans are represented as Symbols for now (placeholder)
// - Strings become ::String

static SymbolTable g_symbol_table;

static SExpression make_nil()
{
    return { .value = ::Nil{} };
}

static SExpression make_string(StringView sv)
{
    return { .value = ::String(sv) };
}

static SExpression make_symbol(StringView sv)
{
    Symbol s = g_symbol_table.intern(sv);

    return { .value = s };
}

static SExpression make_number(StringView sv)
{
    SExpression e;
    // Convert u8string_view to std::string for parsing
    std::string bytes = text_io::to_string(sv);

    try {
        double v = std::stod(bytes);
        e.value = v;
    } catch (...) {
        e.value = 0.0;
    }
    return e;
}

static SExpression make_boolean(StringView sv)
{
    // sv will be like u8"#t" or u8"#f"
    return { .value = (!sv.empty() && sv.size() >= 2 && sv[1] == 't') ? true : false };
}

static SExpression make_char(StringView sv)
{
    SExpression e;

    // Very simple: take first byte as character. Proper UTF-8 decoding
    // can be added later.
    if (!sv.empty())
        e.value = static_cast<char32_t>(sv[0]);
    else
        e.value = static_cast<char32_t>(0);
    return e;
}

static SExpression make_cons(SExpression car, SExpression cdr)
{
    return { .value = cons( std::move(car), std::move(cdr) ) };
}

// Parse the rest of a list given the first token of the first element.
static SExpression parse_list(Tokenizer &tokenizer, Tokenizer::Token t, bool &error)
{
    // If we were handed an EOF token, treat as malformed and return NIL.
    if (t.type == Tokenizer::Type_e::Eof)
    {
        error = true;
        return make_nil();
    }

    // Helper to construct an element from a token
    auto make_element_from_token = [&](Tokenizer::Token tt) -> SExpression {
        if (tt.type == Tokenizer::Type_e::Eof)
        {
            error = true;
            return make_nil();
        }
        switch (tt.type)
        {
            case Tokenizer::Type_e::LeftParen:
        {
            // Next token is first element of nested list (or RightParen)
                Tokenizer::Token nested_first = tokenizer.next_token();

                if (nested_first.type == Tokenizer::Type_e::RightParen)
                    return make_nil();
                return parse_list(tokenizer, nested_first, error);
        }
        case Tokenizer::Type_e::Symbol:
            return make_symbol(tt.text);
        case Tokenizer::Type_e::String:
            return make_string(tt.text);
        case Tokenizer::Type_e::Number:
            return make_number(tt.text);
        case Tokenizer::Type_e::Boolean:
            return make_boolean(tt.text);
        case Tokenizer::Type_e::Char:
            return make_char(tt.text);
        case Tokenizer::Type_e::Quote:
        {
            SExpression quoted = read_expression(tokenizer);
            SExpression qsym = make_symbol(u8"quote");

            return make_cons(qsym, make_cons(quoted, make_nil()));
        }
        default:
            return make_nil();
        }
    };

    SExpression head = make_element_from_token(t);
    auto next = tokenizer.next_token();

    // If we hit EOF while parsing the list, treat as malformed/unterminated
    // and return NIL.
    if (next.type == Tokenizer::Type_e::Eof)
    {
        error = true;
        return make_nil();
    }

    if (next.type == Tokenizer::Type_e::RightParen)
    {
        return make_cons(head, make_nil());
    }
    if (next.type == Tokenizer::Type_e::Dot)
    {
        SExpression cdr = read_expression(tokenizer);
        auto closing = tokenizer.next_token();

        if (closing.type != Tokenizer::Type_e::RightParen)
            return make_nil();
        return make_cons(head, cdr);
    }

    SExpression rest = parse_list(tokenizer, next, error);

    return make_cons(head, rest);
}


SExpression read_expression(Tokenizer &tokenizer)
{
    Tokenizer::Token token = tokenizer.next_token();

    switch (token.type)
    {
    case Tokenizer::Type_e::Eof:
        return make_nil();
    case Tokenizer::Type_e::LeftParen:
    {
        // If next token is RightParen then return empty list (NIL)
        auto next = tokenizer.next_token();

        if (next.type == Tokenizer::Type_e::RightParen)
            return make_nil();

        bool error = false;
        SExpression result = parse_list(tokenizer, next, error);

        if (error)
            return make_nil();
        return result;
    }
    case Tokenizer::Type_e::RightParen:
        // stray right paren; return nil
        return make_nil();
    case Tokenizer::Type_e::Symbol:
        return make_symbol(token.text);
    case Tokenizer::Type_e::String:
        return make_string(token.text);
    case Tokenizer::Type_e::Number:
        return make_number(token.text);
    case Tokenizer::Type_e::Boolean:
        return make_boolean(token.text);
    case Tokenizer::Type_e::Char:
        return make_char(token.text);
    case Tokenizer::Type_e::Quote:
    {
        SExpression quoted = read_expression(tokenizer);
        SExpression qsym = make_symbol(u8"quote");
        SExpression list = make_cons(qsym, make_cons(quoted, make_nil()));

        return list;
    }
    default:
        return make_nil();
    }
}
