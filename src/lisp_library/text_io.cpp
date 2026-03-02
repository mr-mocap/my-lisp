#include <my_lisp/text_io.hpp>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>


namespace text_io
{

std::u8string read_text_file_utf8(const std::filesystem::path &filename)
{
    std::ifstream f(filename, std::ios::binary);

    if ( !f )
        throw std::runtime_error("Failed to open file: " + filename.string());

    std::ostringstream oss;

    oss << f.rdbuf();
    const std::string bytes = oss.str();

    return to_utf8_string(bytes);
}

void write_text_file_utf8(const std::filesystem::path &filename, std::u8string_view content)
{
    std::ofstream f(filename, std::ios::binary | std::ios::trunc);

    if ( !f )
        throw std::runtime_error("Failed to open file for write: " + filename.string());

    std::string_view bytes = to_string_view(content);

    f.write( bytes.data(), static_cast<std::streamsize>( bytes.size() ) );
}

#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <stringapiset.h>

// UTF-8 <-> UTF-16 using Win32 conversions.
// MultiByteToWideChar maps a byte string to UTF-16 and supports CP_UTF8. [1](https://linuxvox.com/blog/what-is-the-linux-equivalent-of-multibytetowidechar-widechartomultibyte/)
static std::wstring utf8_to_utf16(std::u8string_view u8)
{
    if ( u8.empty() )
        return L"";
    const std::string_view bytes = to_string_view(u8);
    int required = MultiByteToWideChar(CP_UTF8,
                                       MB_ERR_INVALID_CHARS,
                                       bytes.data(),
                                       static_cast<int>( bytes.size() ),
                                       nullptr,
                                       0);

    if ( required <= 0 )
        throw std::runtime_error("MultiByteToWideChar failed (UTF-8->UTF-16)."); // [1](https://linuxvox.com/blog/what-is-the-linux-equivalent-of-multibytetowidechar-widechartomultibyte/)

    std::wstring w(required, L'\0');
    int written = MultiByteToWideChar(CP_UTF8,
                                      MB_ERR_INVALID_CHARS,
                                      bytes.data(),
                                      static_cast<int>( bytes.size() ),
                                      w.data(),
                                      required);

    if ( written != required )
        throw std::runtime_error("MultiByteToWideChar short write.");

    return w;
}

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

// Read a UTF-16 line from the Windows console (stdin) and convert to UTF-8.
// This avoids the "std::cin expects current code page bytes" issue on Windows consoles. [9](https://cplusplus.com/forum/general/127273/)
std::u8string read_stdin_line_utf8()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    if ( hIn == INVALID_HANDLE_VALUE )
        throw std::runtime_error("GetStdHandle(STD_INPUT_HANDLE) failed.");

    std::wstring wline;
    wchar_t buf[256];

    while ( true )
    {
        DWORD read = 0;

        if ( !ReadConsoleW(hIn, buf, static_cast<DWORD>(std::size(buf)), &read, nullptr) )
            throw std::runtime_error("ReadConsoleW failed.");

        if ( read == 0 )
            break;

        // ReadConsoleW includes CRLF; collect until newline.
        for ( DWORD i = 0; i < read; ++i )
        {
            wchar_t ch = buf[i];

            if ( ch == L'\r' )
                continue;
            if ( ch == L'\n' )
                return utf16_to_utf8(wline);
            wline.push_back(ch);
        }
    }
    return utf16_to_utf8(wline);
}

void write_stdout_line_utf8(std::u8string_view out)
{
    write_stdout_utf8(out);
}

// Write UTF-8 to Windows console (stdout) via UTF-16 + WriteConsoleW.
// This avoids mojibake when writing UTF-8 bytes to std::cout under a legacy code page. [9](https://cplusplus.com/forum/general/127273/)
void write_stdout_utf8(std::u8string_view out)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if ( hOut == INVALID_HANDLE_VALUE )
        throw std::runtime_error("GetStdHandle(STD_OUTPUT_HANDLE) failed.");

    std::wstring w = utf8_to_utf16(out);
    DWORD written = 0;

    if ( !WriteConsoleW(hOut, w.data(), static_cast<DWORD>(w.size()), &written, nullptr) )
    {
        throw std::runtime_error("WriteConsoleW failed.");
    }
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
#include <iostream>

inline std::u8string read_stdin_line_utf8()
{
    std::string line;

    std::getline(std::cin, line);
    return to_utf8_string(line);
}

void write_stdout_utf8(std::u8string_view out)
{
    std::cout << to_string_view(out);
    std::cout.flush();
}

void write_stdout_line_utf8(std::u8string_view out)
{
    write_stdout_utf8(out);
    std::cout << std::endl;
}

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