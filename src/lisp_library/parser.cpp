#include <my_lisp/parser.hpp>
#include <my_lisp/structures.hpp>
#include <my_lisp/symboltable.hpp>
#include <my_lisp/text_io.hpp>
#include <functional>
#include <expected>

using my_lisp::ParseError;

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

// Forward declaration for the recursive parser helper used by parse_list
static ParseResult read_expression_impl(Tokenizer &tokenizer);

// Parse the rest of a list given the first token of the first element.
static ParseResult parse_list(Tokenizer &tokenizer, Tokenizer::Token t)
{
    // If we were handed an EOF token, treat as malformed and return error.
    if (t.type == Tokenizer::Type_e::Eof)
    {
        return std::unexpected( ParseError{ ParseError::UnterminatedList, t.position, "Unexpected EOF while starting list" } );
    }

    // Helper to construct an element from a token
    auto make_element_from_token = [&](Tokenizer::Token tt) -> std::expected<SExpression, ParseError> {
        if (tt.type == Tokenizer::Type_e::Eof)
        {
            return std::unexpected( ParseError{ ParseError::UnexpectedEOF, tt.position, "Unexpected EOF while parsing element" } );
        }
        switch (tt.type)
        {
        case Tokenizer::Type_e::LeftParen:
        {
            // Next token is first element of nested list (or RightParen)
                Tokenizer::Token nested_first = tokenizer.next_token();

                if (nested_first.type == Tokenizer::Type_e::RightParen)
                    return ParseResult( make_nil() );
                return parse_list(tokenizer, nested_first);
        }
        case Tokenizer::Type_e::Symbol:
            return ParseResult( make_symbol(tt.text) );
        case Tokenizer::Type_e::String:
            return ParseResult( make_string(tt.text) );
        case Tokenizer::Type_e::Number:
            return ParseResult( make_number(tt.text) );
        case Tokenizer::Type_e::Boolean:
            return ParseResult( make_boolean(tt.text) );
        case Tokenizer::Type_e::Char:
            return ParseResult( make_char(tt.text) );
        case Tokenizer::Type_e::Quote:
        {
            ParseResult quoted_res = read_expression_impl(tokenizer);

            if (!quoted_res)
                return std::unexpected( ParseError{ ParseError::UnexpectedToken, tt.position, "Unexpected token after quote" } );

            SExpression qsym = make_symbol(u8"quote");

            return ParseResult( make_cons( qsym, make_cons( quoted_res.value(), make_nil() ) ) );
        }
        default:
            return ParseResult( make_nil() );
        }
    };

    auto head_res = make_element_from_token(t);

    if (!head_res)
        return std::unexpected( head_res.error() );

    SExpression head = head_res.value();
    Tokenizer::Token next = tokenizer.next_token();

    // If we hit EOF while parsing the list, treat as malformed/unterminated
    // and return error.
    if (next.type == Tokenizer::Type_e::Eof)
    {
        return std::unexpected( ParseError{ ParseError::UnterminatedList, next.position, "Unexpected EOF while parsing list" } );
    }

    if (next.type == Tokenizer::Type_e::RightParen)
    {
        return ParseResult( make_cons( head, make_nil() ) );
    }
    if (next.type == Tokenizer::Type_e::Dot)
    {
        auto cdr_res = read_expression_impl(tokenizer);

        if (!cdr_res)
            return std::unexpected( ParseError{ ParseError::MalformedDottedPair, next.position, "Missing cdr after dot" } );

        auto closing = tokenizer.next_token();

        if (closing.type != Tokenizer::Type_e::RightParen)
            return std::unexpected( ParseError{ ParseError::MalformedDottedPair, closing.position, "Missing closing parenthesis after dotted pair" } );

        return ParseResult( make_cons( head, cdr_res.value() ) );
    }

    auto rest_res = parse_list(tokenizer, next);

    if (!rest_res)
        return std::unexpected( rest_res.error() );

    return ParseResult( make_cons( head, rest_res.value() ) );
}


// Forward declaration for use by parse_list and quote handling
static ParseResult read_expression_impl(Tokenizer &tokenizer);

// Internal helper used by the public read_expression; returns expected
static ParseResult read_expression_impl(Tokenizer &tokenizer);

ParseResult read_expression(Tokenizer &tokenizer)
{
    auto res = read_expression_impl(tokenizer);

    if (!res)
        return std::unexpected( res.error() );

    return res;
}


static ParseResult read_expression_impl(Tokenizer &tokenizer)
{
    Tokenizer::Token token = tokenizer.next_token();

    switch (token.type)
    {
    case Tokenizer::Type_e::Eof:
        return ParseResult( make_nil() );
    case Tokenizer::Type_e::LeftParen:
    {
        // If next token is RightParen then return empty list (NIL)
        Tokenizer::Token next = tokenizer.next_token();

        if (next.type == Tokenizer::Type_e::RightParen)
            return ParseResult( make_nil() );

        return parse_list(tokenizer, next);
    }
    case Tokenizer::Type_e::RightParen:
        // stray right paren; return nil
        return ParseResult( make_nil() );
    case Tokenizer::Type_e::Symbol:
        return ParseResult( make_symbol(token.text) );
    case Tokenizer::Type_e::String:
        return ParseResult( make_string(token.text) );
    case Tokenizer::Type_e::Number:
        return ParseResult( make_number(token.text) );
    case Tokenizer::Type_e::Boolean:
        return ParseResult( make_boolean(token.text) );
    case Tokenizer::Type_e::Char:
        return ParseResult( make_char(token.text) );
    case Tokenizer::Type_e::Quote:
    {
        ParseResult quoted_res = read_expression_impl(tokenizer);

        if (!quoted_res)
            return std::unexpected( quoted_res.error() );

        SExpression qsym = make_symbol(u8"quote");

        return ParseResult( make_cons( qsym, make_cons( quoted_res.value(), make_nil() ) ) );
    }
    default:
        return ParseResult( make_nil() );
    }
}
