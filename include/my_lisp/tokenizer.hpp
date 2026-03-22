#pragma once

#include <my_lisp/input.hpp>
#include <cstddef>
#include <variant>


class Tokenizer
{
public:
    enum Type_e
    {
        LeftParen,
        RightParen,
        Symbol,
        Number,
        FixedNumber,
        String,
        Quote,
        Comment,
        Dot,
        Char,
        Eol,
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

    struct CommonData
    {
        StringView text;
        size_t     position = 0;
    };

    struct NumberData
    {
        StringView text;
        size_t     position;
        double     value = 0.0;
    };

    struct FixedNumberData
    {
        StringView text;
        size_t     position = 0;
        int64_t    value = 0;
    };

    struct Token
    {
        Type_e     type;
        std::variant<CommonData, NumberData, FixedNumberData> data;

        StringView text() const
        {
            switch ( type )
            {
            case Number:
                return asNumberData().text;
            case FixedNumber:
                return asFixedNumberData().text;
            default:
                return asCommonData().text;
            }
        }

        size_t position() const
        {
            switch ( type )
            {
            case Number:
                return asNumberData().position;
            case FixedNumber:
                return asFixedNumberData().position;
            default:
                return asCommonData().position;
            }
        }

        const CommonData &asCommonData() const { return std::get<CommonData>(data); }
        const NumberData &asNumberData() const { return std::get<NumberData>(data); }
        const FixedNumberData &asFixedNumberData() const { return std::get<FixedNumberData>(data); }
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
