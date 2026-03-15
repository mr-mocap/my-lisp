#include <my_lisp/tokenizer.hpp>

#include <cctype>
#include <string>

static inline char current_char(const ::String &buf, size_t pos)
{
    if (pos >= buf.size())
        return '\0';
    return static_cast<char>(buf[pos]);
}

Tokenizer::Token Tokenizer::peek()
{
    if (m_has_peek)
        return m_peek_token;

    // Save current state
    ::String old_buffer = m_buffer;
    size_t old_pos = m_pos;
    bool old_eof = m_eof;
    ::String old_storage = m_storage;

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
        return std::isalpha(static_cast<unsigned char>(c)) || std::string("+-*/<=>!?_&%$#@^~").find(c) != std::string::npos;
    }

    bool is_symbol_subsequent(char c)
    {
        return is_symbol_initial(c) || std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == ':';
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

        if (m_eof)
            return Token{ Type_e::Eof, StringView(), m_pos };

        m_buffer = m_reader.read_line();
        m_buffer.push_back(static_cast<char8_t>('\n'));
        m_pos = 0;
        if (m_reader.eof())
            m_eof = true;
    }

    // Skip whitespace and comments
    while (true)
    {
        if (m_pos >= m_buffer.size())
            return next_token();

        char c = current_char(m_buffer, m_pos);

        if (c == ';')
        {
            // Capture comment from ';' to end of line (excluding the newline)
            size_t ofs = m_storage.size();
            size_t comment_start = m_pos;

            // Comment: skip to end of line
            while ( m_pos < m_buffer.size() && current_char(m_buffer, m_pos) != '\n' )
                ++m_pos;

            m_storage.append( StringView(m_buffer.data() + ofs, m_pos - comment_start) );

            return Token{ Type_e::Comment, StringView(m_storage.data() + ofs, m_storage.size()), comment_start};
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
    char c = current_char(m_buffer, m_pos);

    if (c == '(')
    {
        ++m_pos;
        size_t ofs = m_storage.size();
        m_storage.append(u8"(", 1);

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 1);

        return Token{ Type_e::LeftParen, sv, token_pos };
    }
    if (c == ')')
    {
        ++m_pos;
        size_t ofs = m_storage.size();
        m_storage.append(u8")", 1);

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 1);

        return Token{ Type_e::RightParen, sv, token_pos };
    }
    if (c == '\'')
    {
        ++m_pos;
        size_t ofs = m_storage.size();
        m_storage.append(u8"'", 1);

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 1);

        return Token{ Type_e::Quote, sv, token_pos };
    }
    if (c == '.')
    {
        char next = current_char(m_buffer, m_pos + 1);

        if (std::isdigit(static_cast<unsigned char>(next)))
        {
            // fall through to number parsing
        }
        else
        {
            ++m_pos;
            size_t ofs = m_storage.size();
            m_storage.append(u8".", 1);

            StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 1);

            return Token{ Type_e::Dot, sv, token_pos };
        }
    }

    // Strings
    if (c == '"')
    {
        ++m_pos; // consume '"'

        ::String accum;

        while (true)
        {
            if (m_pos >= m_buffer.size())
            {
                if (m_eof)
                    break;

                ::String nextline = m_reader.read_line();

                nextline.push_back(static_cast<char8_t>('\n'));
                m_buffer.append(nextline);
                continue;
            }

            char ch = current_char(m_buffer, m_pos);

            if (ch == '\\')
            {
                ++m_pos;
                if (m_pos >= m_buffer.size())
                    break;

                char esc = current_char(m_buffer, m_pos);

                ++m_pos;
                switch (esc)
                {
                case 'n': accum.push_back(static_cast<char8_t>('\n')); break;
                case 't': accum.push_back(static_cast<char8_t>('\t')); break;
                case '\\': accum.push_back(static_cast<char8_t>('\\')); break;
                case '"': accum.push_back(static_cast<char8_t>('"')); break;
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

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), accum.size());

        return Token{ Type_e::String, sv, token_pos };
    }

    // Booleans and chars
    if (c == '#')
    {
        char next = current_char(m_buffer, m_pos + 1);

        if (next == 't' || next == 'f')
        {
            m_pos += 2;
            size_t ofs = m_storage.size();
            m_storage.append(u8"#", 1);
            m_storage.append(reinterpret_cast<const char8_t *>(&next), 1);

            StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 2);

            return Token{ Type_e::Boolean, sv, token_pos };
        }
        if (next == '\\')
        {
            size_t p = m_pos + 2;

            if (p < m_buffer.size())
            {
                [[maybe_unused]] char ch = current_char(m_buffer, p);
                
                ++p;
                size_t ofs = m_storage.size();

                m_storage.append(m_buffer.data() + p - 1, 1);
                m_pos = p;

                return Token{ Type_e::Char, StringView(m_storage.data() + ofs, 1), token_pos};
            }
        }
    }

    // Numbers
    if (c == '+' || c == '-' || std::isdigit(static_cast<unsigned char>(c)) || (c == '.' && std::isdigit(static_cast<unsigned char>(current_char(m_buffer, m_pos+1)))))
    {
        size_t start = m_pos;

        if (c == '+' || c == '-')
            ++m_pos;

        while (std::isdigit(static_cast<unsigned char>(current_char(m_buffer, m_pos))))
            ++m_pos;

        if (current_char(m_buffer, m_pos) == '.')
        {
            ++m_pos;
            while (std::isdigit(static_cast<unsigned char>(current_char(m_buffer, m_pos))))
                ++m_pos;
        }

        size_t len = m_pos - start;
        size_t ofs = m_storage.size();

        m_storage.append(reinterpret_cast<const char8_t *>(m_buffer.data() + start), len);

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), len);

        return Token{ Type_e::Number, sv, token_pos };
    }

    // Symbol
    if (is_symbol_initial(c))
    {
        size_t start = m_pos;

        ++m_pos;
        while (is_symbol_subsequent(current_char(m_buffer, m_pos)))
            ++m_pos;

        size_t len = m_pos - start;
        size_t ofs = m_storage.size();

        m_storage.append(reinterpret_cast<const char8_t *>(m_buffer.data() + start), len);

        StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), len);

        return Token{ Type_e::Symbol, sv, token_pos };
    }

    // Fallback: treat unknown single byte as symbol
    ++m_pos;

    size_t ofs = m_storage.size();

    m_storage.append(reinterpret_cast<const char8_t *>(m_buffer.data() + token_pos), 1);

    StringView sv(reinterpret_cast<const char8_t *>(m_storage.data() + ofs), 1);

    return Token{ Type_e::Symbol, sv, token_pos };
}
