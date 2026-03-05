#include <my_lisp/text_io.hpp>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>


namespace text_io
{

std::u8string read_line_utf8_from(std::istream &input)
{
    std::string line;

    if ( !std::getline(input, line) )
    {
        // EOF or error.
        // Caller can check input.eof() or input.fail() if needed.
        // We return empty string on both cases, as an empty line is a valid input.
        return {};
    }
    return to_utf8_string(line);
}

void write_utf8_to(std::ostream &output, std::u8string_view content)
{
    std::string_view bytes = to_string_view(content);

    output.write( bytes.data(), static_cast<std::streamsize>( bytes.size() ) );

    // Check for errors if needed (caller) (e.g., output.fail()).
}

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <stringapiset.h>

static std::u8string utf16_to_utf8(std::wstring_view w)
{
    if ( w.empty() )
        return {};

    int required = WideCharToMultiByte(CP_UTF8,
                                       WC_ERR_INVALID_CHARS,
                                       w.data(),
                                       static_cast<int>(w.size()),
                                       nullptr,
                                       0,
                                       nullptr,
                                       nullptr);

    if ( required <= 0 )
        throw std::runtime_error("WideCharToMultiByte failed (UTF-16->UTF-8)."); // Paired with MultiByteToWideChar usage pattern. [2](https://www.codegenes.net/blog/how-to-print-utf-8-strings-to-std-cout-on-windows/)[1](https://linuxvox.com/blog/what-is-the-linux-equivalent-of-multibytetowidechar-widechartomultibyte/)

    std::string bytes(required, '\0');
    int written = WideCharToMultiByte(CP_UTF8,
                                      WC_ERR_INVALID_CHARS,
                                      w.data(),
                                      static_cast<int>(w.size()),
                                      bytes.data(),
                                      required,
                                      nullptr,
                                      nullptr);

    if ( written != required )
        throw std::runtime_error("WideCharToMultiByte short write.");

    return to_utf8_string(bytes);
}

// Convert Windows wide argv (UTF-16) to UTF-8 early.
// wmain is the robust path for Unicode argv on Windows. [7](https://github.com/ww898/utf-cpp)[8](https://stackoverflow.com/questions/2527720/confused-about-cs-stdwstring-utf-16-utf-8-and-displaying-strings-in-a-win)
std::vector<std::u8string> argv_to_utf8(int argc, wchar_t *argv[])
{
    std::vector<std::u8string> out;

    out.reserve( static_cast<size_t>(argc) );
    for ( int i = 0; i < argc; ++i )
        out.push_back( utf16_to_utf8(argv[i] ? std::wstring_view(argv[i]) : std::wstring_view{}) );
    return out;
}
#else
// Unix-like: treat stdin/stdout as UTF-8 bytes (common when locale is UTF-8). [4](https://github.com/mbits-libs/utfconv)[5](https://learnmoderncpp.com/2021/03/24/a-unicode-primer/)
std::vector<std::u8string> argv_to_utf8(int argc, char *argv[])
{
    std::vector<std::u8string> out;

    out.reserve( static_cast<size_t>(argc) );
    for ( int i = 0; i < argc; ++i )
    {
        std::string_view sv = argv[i] ? std::string_view(argv[i]) : std::string_view{};

        out.push_back( to_utf8_string_view(sv) );
    }
    return out;
}
#endif
}