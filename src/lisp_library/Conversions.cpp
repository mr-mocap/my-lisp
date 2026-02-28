#include <my_lisp/Conversions.hpp>
#include <span>
#include <stdexcept>


namespace
{

static inline bool is_cont(uint8_t b)
{
    return (b & 0xC0u) == 0x80u;
}


struct DecodeResult
{
    bool     success    = false;
    int      length     = 0; // number of bytes in the sequence
    char32_t code_point = 0; // valid if success == true
};

// Decodes one UTF-8 scalar value from [p, end).
// On success: writes code point to out, advances p, returns true.
// On error: does not advance p (except for policy handling outside), returns false.
DecodeResult decode_one_utf8(std::u8string_view p)
{
    if ( p.empty() )
        return { .success = false };

    uint8_t b0 = static_cast<uint8_t>( p.front() );

    // 1-byte ASCII
    if ( b0 < 0x80 )
    {
        return { .success = true, .length = 1, .code_point = static_cast<char32_t>(b0) };
    }

    // Determine expected length and initial payload
    int      len = 0;
    char32_t cp = 0;

    if ( (b0 & 0xE0u) == 0xC0u )
    {
        len = 2;
        cp = b0 & 0x1Fu;
    }
    else if ( (b0 & 0xF0u) == 0xE0u )
    {
        len = 3;
        cp = b0 & 0x0Fu;
    }
    else if ( (b0 & 0xF8u) == 0xF0u )
    {
        len = 4;
        cp = b0 & 0x07u;
    }
    else
    {
        return { .success = false }; // invalid leading byte
    }

    if ( std::ssize(p) < len )
        return { .success = false }; // incomplete sequence

    // Accumulate continuation bytes
    for ( int i = 1; i < len; ++i )
    {
        uint8_t bi = p[i];

        if ( !is_cont(bi) )
            return { .success = false };
        cp = (cp << 6) | (bi & 0x3Fu);
    }

    // Reject overlong encodings
    // Minimum code point representable with len bytes:
    // len=2 -> 0x80, len=3 -> 0x800, len=4 -> 0x10000
    static constexpr char32_t min_cp[5] = { 0, 0, 0x80, 0x800, 0x10000 };

    if ( cp < min_cp[len] )
        return { .success = false };

    // Reject UTF-16 surrogate halves (U+D800..U+DFFF)
    if ( cp >= 0xD800 && cp <= 0xDFFF )
        return { .success = false };

    // Reject out-of-range (> U+10FFFF)
    if ( cp > 0x10FFFF )
        return { .success = false };

    return { .success = true, .length = len, .code_point = cp };
}

}

std::u32string utf8_to_u32(std::u8string_view s, Utf8ErrorPolicy policy)
{
    std::u32string out;

    out.reserve( s.size() ); // upper bound (worst case 1 byte -> 1 code point)

    while ( !s.empty() )
    {
        if ( auto result = decode_one_utf8( s ); result.success )
        {
            out.push_back( result.code_point );
            s.remove_prefix( result.length );
            continue;
        }

        // Error handling
        switch ( policy )
        {
        case Utf8ErrorPolicy::Replace:
            out.push_back(U'\uFFFD');
            // Skip one byte to avoid infinite loop
            s.remove_prefix( 1 );
            break;
        case Utf8ErrorPolicy::Throw:
            throw std::runtime_error("Invalid UTF-8 sequence");
        case Utf8ErrorPolicy::Stop:
            return out;
        }
    }
    return out;
}
