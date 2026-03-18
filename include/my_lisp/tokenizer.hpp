#pragma once

#include <my_lisp/input.hpp>
#include <cstddef>


class Tokenizer
{
public:
    enum Type_e
    {
        LeftParen,
        RightParen,
        Symbol,
        Number,
        String,
        Quote,
        Comment,
        Dot,
        Char,
        Eof
    };

    Tokenizer(Input &input)
        :
        m_input(input)
    {
    }

    // We have a reference member, so we must delete copy operations
    Tokenizer(const Tokenizer &) = delete;
    Tokenizer &operator=(const Tokenizer &) = delete;

    struct Token
    {
        Type_e     type;
        StringView text;
        size_t     position;
    };

    // Read the next token from the input. This will pull lines from the
    // provided `Input` as needed. On end-of-file, a Token with type `EOF`
    // will be returned.
    Token next_token();

    // Peek the next token without consuming it. Subsequent call to
    // `next_token()` will return the same token.
    Token peek();

protected:
    Input &m_input;

    // Current input buffer (one or more lines concatenated as needed)
    ::String m_buffer;
    size_t   m_pos = 0;
    bool     m_eof = false;
    bool     m_read_attempted = false;

    // Persistent storage for token text to ensure StringView remains valid
    // after m_buffer is replaced.
    ::String m_storage;

    // Peek cache
    bool     m_has_peek = false;
    Token    m_peek_token;
    ::String m_peek_buffer;
    size_t   m_peek_pos = 0;
    bool     m_peek_eof = false;
    ::String m_peek_storage;
};
