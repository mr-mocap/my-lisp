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
    SExpression e;
    e.value = ::Nil{};
    return e;
}

static SExpression make_string(StringView sv)
{
    SExpression e;
    e.value = ::String(sv);
    return e;
}

static SExpression make_symbol(StringView sv)
{
    SExpression e;
    Symbol s = g_symbol_table.intern(sv);
    e.value = s;
    return e;
}

static SExpression make_cons(SExpression car, SExpression cdr)
{
    SExpression e;
    e.value = cons(std::move(car), std::move(cdr));
    return e;
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
        // parse list
        // If next token is RightParen then return empty list (NIL)
        auto next = tokenizer.next_token();
        if (next.type == Tokenizer::Type_e::RightParen)
            return make_nil();

        // Otherwise parse first element and then the rest
        // We need to push back the token since read_expression expects to see it.
        // Simple approach: create a tiny buffer by reconstructing a tokenizer is complex.
        // Instead, implement a helper that takes the first token already read.
        // For simplicity, we'll implement a small recursive lambda that accepts the first token.

        std::function<SExpression(Tokenizer::Token)> parse_list = [&](Tokenizer::Token t) -> SExpression {
            // parse head element from token t
            // We need a way to parse an expression given a token already consumed. To keep
            // this minimal, if t is a LeftParen we'll recursively call read_expression which
            // will call tokenizer.next_token() as needed. For other tokens construct directly.

            SExpression head;
            switch (t.type)
            {
            case Tokenizer::Type_e::LeftParen:
                head = read_expression(tokenizer);
                break;
            case Tokenizer::Type_e::Symbol:
                head = make_symbol(t.text);
                break;
            case Tokenizer::Type_e::String:
                head = make_string(t.text);
                break;
            case Tokenizer::Type_e::Number:
                // represent numbers as symbol containing the text for now
                head = make_symbol(t.text);
                break;
            case Tokenizer::Type_e::Boolean:
                head = make_symbol(t.text);
                break;
            case Tokenizer::Type_e::Char:
                head = make_symbol(t.text);
                break;
            case Tokenizer::Type_e::Quote:
                // quote followed by expression -> (quote expr)
                {
                    SExpression quoted = read_expression(tokenizer);
                    SExpression qsym = make_symbol(u8"quote");
                    SExpression list = make_cons(qsym, make_cons(quoted, make_nil()));
                    return list;
                }
            default:
                head = make_nil();
                break;
            }

            // Now parse the rest of the list
            auto next = tokenizer.next_token();
            if (next.type == Tokenizer::Type_e::RightParen)
            {
                return make_cons(head, make_nil());
            }
            else if (next.type == Tokenizer::Type_e::Dot)
            {
                // dotted pair: next token is the cdr expression, followed by RightParen
                SExpression cdr = read_expression(tokenizer);
                auto closing = tokenizer.next_token();
                if (closing.type != Tokenizer::Type_e::RightParen)
                {
                    // malformed; return nil
                    return make_nil();
                }
                return make_cons(head, cdr);
            }
            else
            {
                // next is the beginning of the rest of the list; recursively parse
                SExpression rest = parse_list(next);
                return make_cons(head, rest);
            }
        };

        // We already read 'next' above which is the token after '('
        return parse_list(next);
    }
    case Tokenizer::Type_e::RightParen:
        // stray right paren; return nil
        return make_nil();
    case Tokenizer::Type_e::Symbol:
        return make_symbol(token.text);
    case Tokenizer::Type_e::String:
        return make_string(token.text);
    case Tokenizer::Type_e::Number:
        return make_symbol(token.text);
    case Tokenizer::Type_e::Boolean:
        return make_symbol(token.text);
    case Tokenizer::Type_e::Char:
        return make_symbol(token.text);
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
