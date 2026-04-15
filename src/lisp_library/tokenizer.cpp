#include <my_lisp/tokenizer.hpp>
#include <cctype>
#include <string>
#include <span>
#include <charconv>
#include <cassert>


static inline char current_char(const FundamentalType::String &buf, size_t pos)
{
    if (pos >= buf.size())
        return '\0';
    return static_cast<char>(buf[pos]);
}

static inline unsigned char current_char_as_uc(const FundamentalType::String &buf, size_t pos)
{
    if (pos >= buf.size())
        return '\0';
    return static_cast<unsigned char>(buf[pos]);
}

static inline FundamentalType::String uppercase_symbol(FundamentalType::StringView input)
{
    // TODO: FIXME: This only works for ASCII characters. Proper Unicode case folding can be added later.
    FundamentalType::String result;

    result.reserve( input.size() );
    for ( char8_t c : input )
    {
        int uc = std::toupper( static_cast<unsigned char>(c) );

        result.append(1, static_cast<char8_t>(uc));
    }
    return result;
}

static inline int is_base_integer_specifier(char c)
{
    switch ( c )
    {
    case 'b':
        return 2;
    case 'o':
        return 8;
    case 'x':
        return 16;
    }
    return 0; // Not recoognized
}

static bool is_digit_in_given_base(char c, int base)
{
    if ( base <= 10 )
        return std::isdigit(c);
    else if ( base == 16 )
        return std::isxdigit(c);
    return false; // Not a recognized base
}

Tokenizer::Token make_common(Tokenizer::Type_e t, FundamentalType::StringView sv, size_t pos)
{
    return { .type = t, .data = Tokenizer::CommonData{ .text = sv, .position = pos } };
}

Tokenizer::Token make_leftparen(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::LeftParen, sv, pos);
}

Tokenizer::Token make_rightparen(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::RightParen, sv, pos);
}

Tokenizer::Token make_symbol(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::Symbol, sv, pos);
}

Tokenizer::Token make_number(FundamentalType::StringView sv, size_t pos, double v)
{
    return { .type = Tokenizer::Type_e::Number, .data = Tokenizer::NumberData{.text = sv, .position = pos, .value = v } };
}

Tokenizer::Token make_fixednumber(FundamentalType::StringView sv, size_t pos, int64_t v)
{
    return { .type = Tokenizer::Type_e::FixedNumber, .data = Tokenizer::FixedNumberData{ .text = sv, .position = pos, .value = v } };
}

Tokenizer::Token make_string(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::String, sv, pos);
}

Tokenizer::Token make_quote(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::Quote, sv, pos);
}

Tokenizer::Token make_comment(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::Comment, sv, pos);
}

Tokenizer::Token make_dot(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::Dot, sv, pos);
}

Tokenizer::Token make_char(FundamentalType::StringView sv, size_t pos)
{
    return make_common(Tokenizer::Type_e::Char, sv, pos);
}

Tokenizer::Token make_eof(size_t pos)
{
    return make_common(Tokenizer::Type_e::Eof, FundamentalType::StringView(), pos);
}

Tokenizer::Token Tokenizer::peek()
{
    if (m_has_peek)
        return m_peek_token;

    // Save current state
    FundamentalType::String old_buffer = m_buffer;
    size_t old_pos = m_pos;
    bool old_eof = m_eof;
    FundamentalType::String old_storage = m_storage;

    // Consume a token to advance internal state
    Token t = next_token();

    // Save the advanced state into peek storage
    m_peek_token = t;
    m_peek_buffer = m_buffer;
    m_peek_pos = m_pos;
    m_peek_eof = m_eof;
    m_peek_storage = m_storage;
    m_has_peek = true;

    // Restore original state so token remains unconsumed
    m_buffer = std::move(old_buffer);
    m_pos = old_pos;
    m_eof = old_eof;
    m_storage = std::move(old_storage);

    return m_peek_token;
}

namespace {

bool is_symbol_initial(char c)
{
    return std::isalpha(static_cast<unsigned char>(c)) ||
           std::string_view("+-*/<=>!?_&%$#@^~").find(c) != std::string::npos;
}

bool is_symbol_initial_uc(unsigned char c)
{
    return std::isalpha(c) || std::string_view("+-*/<=>!?_&%$#@^~").find(c) != std::string::npos;
}

bool is_symbol_subsequent_uc(unsigned char c)
{
    return is_symbol_initial_uc(c) || std::isdigit(c) || c == '.' || c == ':';
}

}

Tokenizer::Token Tokenizer::next_token()
{
    if (m_has_peek)
    {
        m_has_peek = false;
        // move peeked token into return value and restore internal buffers
        Token t = m_peek_token;

        m_storage = std::move(m_peek_storage);
        m_buffer = std::move(m_peek_buffer);
        m_pos = m_peek_pos;
        m_eof = m_peek_eof;
        return t;
    }

    // Ensure buffer has data or try to read a line
    for (;;)
    {
        if (m_pos < m_buffer.size())
            break;

        if ( m_eof )
            return make_eof( m_pos );

        m_buffer = m_input.read_line();
        m_buffer.push_back( u8'\n' );
        m_pos = 0;
        if (m_input.eof())
            m_eof = true;
    }

    // Skip whitespace and comments
    while (true)
    {
        if (m_pos >= m_buffer.size())
            return next_token();

        const char c = current_char(m_buffer, m_pos);

        if (c == ';')
        {
            // Capture comment from ';' to end of line (excluding the newline)
            size_t ofs = m_storage.size();
            size_t comment_start = m_pos;

            // Comment: skip to end of line
            while ( m_pos < m_buffer.size() && current_char(m_buffer, m_pos) != '\n' )
                ++m_pos;

            m_storage.append(FundamentalType::StringView(m_buffer.data() + ofs, m_pos - comment_start));
            return make_comment( FundamentalType::StringView(m_storage.data() + ofs, m_storage.size()), comment_start );
        }

        if ( std::isspace(static_cast<unsigned char>(c)) )
        {
            // Skip whitespace
            ++m_pos;
            continue;
        }
        break;
    }

    size_t token_pos = m_pos;
    const char          c  = current_char(m_buffer, m_pos);
    const unsigned char uc = current_char_as_uc(m_buffer, m_pos);

    if (c == '(')
    {
        ++m_pos;
        size_t ofs = m_storage.size();

        m_storage.append(1, u8'(');

        return make_leftparen( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
    }
    if (c == ')')
    {
        ++m_pos;
        size_t ofs = m_storage.size();

        m_storage.append(1, u8')');

        return make_rightparen( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
    }
    if (c == '\'')
    {
        ++m_pos;
        size_t ofs = m_storage.size();

        m_storage.append(1, u8'\'');

        return make_quote( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
    }
    if (c == '.')
    {
        unsigned char next = current_char_as_uc(m_buffer, m_pos + 1);

        if ( std::isdigit(next) )
        {
            // fall through to number parsing
        }
        else
        {
            ++m_pos;
            size_t ofs = m_storage.size();

            m_storage.append(1, u8'.');

            return make_dot( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
        }
    }

    // Strings
    if (c == '"')
    {
        ++m_pos; // consume '"'

        FundamentalType::String accum;

        while (true)
        {
            if (m_pos >= m_buffer.size())
            {
                if (m_eof)
                    break;

                FundamentalType::String nextline = m_input.read_line();

                nextline.push_back( u8'\n' );
                m_buffer.append(nextline);
                continue;
            }

            char ch = current_char(m_buffer, m_pos);

            if (ch == '\\')
            {
                ++m_pos;
                if (m_pos >= m_buffer.size())
                    break;

                const char esc = current_char(m_buffer, m_pos);

                ++m_pos;
                switch (esc)
                {
                case 'n': accum.push_back( u8'\n' ); break;
                case 't': accum.push_back( u8'\t' ); break;
                case '\\': accum.push_back( u8'\\' ); break;
                case '"': accum.push_back( u8'"' ); break;
                default:
                    accum.push_back(static_cast<char8_t>(esc));
                    break;
                }
                continue;
            }

            if (ch == '"')
            {
                ++m_pos; // consume closing
                break;
            }

            accum.push_back(static_cast<char8_t>(ch));
            ++m_pos;
        }

        // store into persistent storage so returned view remains valid
        size_t ofs = m_storage.size();
        m_storage.append(accum);

        return make_string( FundamentalType::StringView(m_storage.data() + ofs, accum.size()), token_pos );
    }

    // chars
    if (c == '#')
    {
        const char next = current_char(m_buffer, m_pos + 1);

        if (next == '\\')
        {
            size_t p = m_pos + 2;

            if (p < m_buffer.size())
            {
                ++p;
                size_t ofs = m_storage.size();

                m_storage.append(m_buffer.data() + p - 1, 1);
                m_pos = p;

                return make_char( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
            }
        }
        else if ( int base = is_base_integer_specifier(next) )
        {
            m_pos += 2;

            size_t first_pos = m_pos;
            size_t ofs = m_storage.size();

            while ( is_digit_in_given_base( current_char(m_buffer, m_pos), base ) )
                ++m_pos;

            size_t last_pos = m_pos;
            const char *start = reinterpret_cast<const char *>(m_buffer.data() + first_pos);
            const char *end   = reinterpret_cast<const char *>(m_buffer.data() + last_pos);
            int64_t     result;
            std::from_chars_result conversion = std::from_chars(start, end, result, base);

            if ( conversion.ec == std::errc() )
            {
                // ASSUME: All characters matched
                m_storage.append(m_buffer.data() + first_pos, m_buffer.data() + last_pos);
                return make_fixednumber( FundamentalType::StringView(m_storage.data() + ofs, last_pos - first_pos), token_pos, result );
            }
            else
            {
                // Error
                // TODO: Implement
                assert(false);
            }
        }
    }

    // Number or FixedNumber
    if (c == '+' || c == '-' || std::isdigit(uc) || (c == '.' && std::isdigit( current_char_as_uc(m_buffer, m_pos+1) )))
    {
        Type_e token_type = Type_e::FixedNumber; // Let's default to this and change it when we find a '.'
        size_t start = m_pos;

        if (c == '+' || c == '-')
            ++m_pos;

        while ( std::isdigit( current_char_as_uc(m_buffer, m_pos) ) )
            ++m_pos;

        if (current_char(m_buffer, m_pos) == '.')
        {
            token_type = Type_e::Number;
            ++m_pos;
            while ( std::isdigit( current_char_as_uc(m_buffer, m_pos) ) )
                ++m_pos;
        }

        size_t len = m_pos - start;
        size_t ofs = m_storage.size();

        m_storage.append(m_buffer.data() + start, len);

        if ( token_type == FixedNumber )
        {
            const char *number_start = reinterpret_cast<const char *>(m_buffer.data() + start);
            const char *number_end   = reinterpret_cast<const char *>(m_buffer.data() + m_pos);
            int64_t     result;
            std::from_chars_result conversion;

            if ( c == '+' )
                conversion = std::from_chars(number_start + 1, number_end, result, 10); // from_chars does NOT handle the '+', so remove it from the parse!
            else
                conversion = std::from_chars(number_start, number_end, result, 10);

            if ( conversion.ec == std::errc() )
            {
                // ASSUME: All characters matched
                return make_fixednumber( FundamentalType::StringView(m_storage.data() + ofs, len), token_pos, result );
            }
            else
            {
                // Error
                // TODO: Implement
                // Unparsable Number!
                assert(false);
            }
        }
        else
        {
            assert(token_type == Number);

            const char *number_start = reinterpret_cast<const char *>(m_buffer.data() + start);
            const char *number_end   = reinterpret_cast<const char *>(m_buffer.data() + m_pos);
            double      result;
            std::from_chars_result conversion = std::from_chars(number_start, number_end, result );

            if ( conversion.ec == std::errc() )
            {
                // ASSUME: All characters matched
                return make_number( FundamentalType::StringView(m_storage.data() + ofs, len), token_pos, result );
            }
            else
            {
                // Error
                // TODO: Implement
                assert(false);
            }
        }
    }

    // Symbol
    if ( is_symbol_initial(c) )
    {
        size_t start = m_pos;

        ++m_pos;
        while ( is_symbol_subsequent_uc( current_char_as_uc(m_buffer, m_pos) ) )
            ++m_pos;

        size_t len = m_pos - start;
        size_t ofs = m_storage.size();

        FundamentalType::String uppercased_symbol = uppercase_symbol( FundamentalType::StringView(m_buffer.data() + start, len) );

        m_storage.append( uppercased_symbol );

        if ( uppercased_symbol == u8"EXIT" )
            return make_eof( m_pos );
        return make_symbol( FundamentalType::StringView(m_storage.data() + ofs, len), token_pos );
    }

    // Fallback: treat unknown single byte as symbol
    ++m_pos;

    size_t ofs = m_storage.size();

    m_storage.append(m_buffer.data() + token_pos, 1);

    return make_symbol( FundamentalType::StringView(m_storage.data() + ofs, 1), token_pos );
}
