#include <my_lisp/parser.hpp>
#include <my_lisp/structures.hpp>
#include <my_lisp/symboltable.hpp>
#include <my_lisp/text_io.hpp>
#include <my_lisp/tokenizer.hpp>
#include <expected>



static ParseResult read_expression_impl(Tokenizer &tokenizer, Environment &environment);

// Minimal parser that consumes tokens and builds SExpression values.
// - Symbols are interned via a global SymbolTable instance (simple).
// - Numbers and booleans are represented as Symbols for now (placeholder)
// - Strings become ::String

static SExpression make_symbol(StringView sv, Environment &environment)
{
    OptionalSymbol sym_opt = environment.find_symbol(sv);

    if ( sym_opt )
        return { sym_opt.value() };
    else
        return { environment.current_package()->intern(sv) };
}

// Try to parse a numeric literal. On failure return a ParseError instead of
// silently returning 0.0.
static ParseResult make_number(const Tokenizer::NumberData &data)
{
    return ParseResult( SExpression{ data.value } );
#if 0
    // Convert u8string_view to std::string for parsing
    std::string bytes = text_io::to_string(data.text);

    try
    {
        size_t idx = 0;
        Number v = std::stod(bytes, &idx);

        // Ensure the entire token was consumed by the number parser
        if (idx != bytes.size())
            return std::unexpected( ParseError{ ParseError::UnexpectedToken, 0, "Invalid number literal" } );

    }
    catch (...)
    {
        return std::unexpected( ParseError{ ParseError::UnexpectedToken, 0, "Invalid number literal" } );
    }
#endif
}

static ParseResult make_fixednumber(const Tokenizer::FixedNumberData &data)
{
    return ParseResult( SExpression{ data.value } );
#if 0
    // Convert u8string_view to std::string for parsing
    std::string bytes = text_io::to_string(data.text);

    try
    {
        size_t idx = 0;
        Number v = std::stoi(bytes, &idx);

        // Ensure the entire token was consumed by the number parser
        if (idx != bytes.size())
            return std::unexpected( ParseError{ ParseError::UnexpectedToken, 0, "Invalid number literal" } );

        return ParseResult( SExpression{ v } );
    }
    catch (...)
    {
        return std::unexpected( ParseError{ ParseError::UnexpectedToken, 0, "Invalid number literal" } );
    }
#endif
}

static ParseResult make_char(StringView sv)
{
    if (sv.empty())
        return std::unexpected( ParseError{ ParseError::UnexpectedToken, 0, "Empty char literal" } );

    // Very simple: take first byte as character. Proper UTF-8 decoding
    // can be added later.
    return ParseResult( SExpression{ static_cast<Char>(sv[0]) } );
}

// Parse the rest of a list given the first token of the first element.
static ParseResult parse_list(Tokenizer &tokenizer, Tokenizer::Token t, Environment &environment)
{
    // If we were handed an EOF token, treat as malformed and return error.
    if (t.type == Tokenizer::Type_e::Eof)
    {
        return std::unexpected( ParseError{ ParseError::UnterminatedList, t.position(), "Unexpected EOF while starting list"});
    }

    // Helper to construct an element from a token
    auto make_element_from_token = [&](Tokenizer::Token tt) -> std::expected<SExpression, ParseError> {
        if (tt.type == Tokenizer::Type_e::Eof)
        {
            return std::unexpected( ParseError{ ParseError::UnexpectedEOF, tt.position(), "Unexpected EOF while parsing element"});
        }
        switch (tt.type)
        {
        case Tokenizer::Type_e::LeftParen:
        {
            // Next token is first element of nested list (or RightParen)
                Tokenizer::Token nested_first = tokenizer.next_token();

                if (nested_first.type == Tokenizer::Type_e::RightParen)
                    return ParseResult( SExpression::make_nil() );
                return parse_list(tokenizer, nested_first, environment);
        }
        case Tokenizer::Type_e::Symbol:
            return ParseResult( make_symbol(tt.text(), environment));

        case Tokenizer::Type_e::String:
            return ParseResult( SExpression(tt.text()) );

        case Tokenizer::Type_e::Number:
        {
            auto num_res = make_number( tt.asNumberData() );

            if (!num_res)
                return std::unexpected( num_res.error() );
            return num_res;
        }
        case Tokenizer::Type_e::FixedNumber:
        {
            auto num_res = make_fixednumber( tt.asFixedNumberData() );

            if (!num_res)
                return std::unexpected( num_res.error() );
            return num_res;
        }
        case Tokenizer::Type_e::Char:
        {
            auto cres = make_char(tt.text());

            if (!cres)
                return std::unexpected( cres.error() );
            return cres;
        }
        case Tokenizer::Type_e::Quote:
        {
            ParseResult quoted_res = read_expression_impl(tokenizer, environment);

            if (!quoted_res)
                return std::unexpected( ParseError{ ParseError::UnexpectedToken, tt.position(), "Unexpected token after quote"});

            SExpression qsym = make_symbol(u8"quote", environment);

            return ParseResult( SExpression::make_cons( qsym, SExpression::make_cons( quoted_res.value(), SExpression::make_nil() ) ) );
        }
        default:
            return std::unexpected( ParseError{ ParseError::UnexpectedToken, tt.position(), "Unexpected token while parsing element"});
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
        return std::unexpected( ParseError{ ParseError::UnterminatedList, next.position(), "Unexpected EOF while parsing list"});
    }

    if (next.type == Tokenizer::Type_e::RightParen)
    {
        return ParseResult( SExpression::make_cons( head, SExpression::make_nil() ) );
    }
    if (next.type == Tokenizer::Type_e::Dot)
    {
        auto cdr_res = read_expression_impl(tokenizer, environment);

        if (!cdr_res)
            return std::unexpected( ParseError{ ParseError::MalformedDottedPair, next.position(), "Missing cdr after dot"});

        auto closing = tokenizer.next_token();

        if (closing.type != Tokenizer::Type_e::RightParen)
            return std::unexpected( ParseError{ ParseError::MalformedDottedPair, closing.position(), "Missing closing parenthesis after dotted pair"});

        return ParseResult( SExpression::make_cons( head, cdr_res.value() ) );
    }

    auto rest_res = parse_list(tokenizer, next, environment);

    if (!rest_res)
        return std::unexpected( rest_res.error() );

    return ParseResult( SExpression::make_cons( head, rest_res.value() ) );
}


static ParseResult read_expression_impl(Tokenizer &tokenizer, Environment &environment)
{
    Tokenizer::Token token = tokenizer.next_token();

    switch (token.type)
    {
    case Tokenizer::Type_e::Eof:
        // No expression available at top-level: treat as EOF
        return std::unexpected( ParseError{ ParseError::UnexpectedEOF, token.position(), "Unexpected EOF at top-level"});

    case Tokenizer::Type_e::Eol:
        return std::unexpected( ParseError{ ParseError::EOL, token.position(), "End-Of-Line (need more input)"});

    case Tokenizer::Type_e::LeftParen:
    {
        // If next token is RightParen then return empty list (NIL)
        Tokenizer::Token next = tokenizer.next_token();

        if (next.type == Tokenizer::Type_e::RightParen)
            return ParseResult( SExpression::make_nil() );

        return parse_list(tokenizer, next, environment);
    }
    case Tokenizer::Type_e::RightParen:
        // stray right paren — return an explicit parse error instead of NIL
        return std::unexpected( ParseError{ ParseError::UnexpectedToken, token.position(), "Stray right parenthesis"});

    case Tokenizer::Type_e::Symbol:
        return ParseResult( make_symbol(token.text(), environment) );

    case Tokenizer::Type_e::String:
        return ParseResult( SExpression(token.text()) );

    case Tokenizer::Type_e::Number:
        return make_number( token.asNumberData() );

    case Tokenizer::Type_e::FixedNumber:
        return make_fixednumber( token.asFixedNumberData() );

    case Tokenizer::Type_e::Char:
        return make_char(token.text());

    case Tokenizer::Type_e::Quote:
    {
        ParseResult quoted_res = read_expression_impl(tokenizer, environment);

        if (!quoted_res)
            return std::unexpected( quoted_res.error() );

        SExpression qsym = make_symbol(u8"quote", environment);

        return ParseResult( SExpression::make_cons( qsym, SExpression::make_cons( quoted_res.value(), SExpression::make_nil() ) ) );
    }
    default:
        // Unexpected token at top-level
        return std::unexpected( ParseError{ ParseError::UnexpectedToken, token.position(), "Unexpected token"});
    }
}

ParseResult read_expression(Tokenizer &tokenizer, Environment &environment)
{
    ParseResult res = read_expression_impl(tokenizer, environment);

    if (!res)
        return std::unexpected( res.error() );

    return res;
}
