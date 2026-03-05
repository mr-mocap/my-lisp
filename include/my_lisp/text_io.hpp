#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <iostream>

/**
 *  Text I/O Boundary
 *
 *  We are going to treat text as UTF‑8 internally, so we need to convert to/from UTF‑8 at the boundaries
 *  of our system.  Here are the main boundaries we need to consider:
 *
 *  CLI arguments boundary (Windows special case)
 *
 *  Provide both entry points :
 *
 *  Windows: wmain(...) → convert args to UTF‑8 early.[iifx.dev], [learn.microsoft.com]
 *  Others : main(...) (typically already UTF‑8 in UTF‑8 locales)
 *
 * 
 *  Terminal Boundary
 *   
 *  std::u8string read_stdin_utf8()
 *      * Unix    : read bytes, assume UTF‑8 if locale says UTF‑8; else error / override.[baeldung.com], [wiki.archlinux.org]
 *      * Windows : read wide from console when possible, convert UTF‑16→UTF‑8. (If you stay byte - based, you inherit codepage headaches.)
 *
 *
 *  File I/O Boundary
 *  
 *  std::u8string read_text_file_utf8(const std::path &filename);
 *  void write_text_file_utf8(const std::path &filename, std::u8string_view content);
 */

/**
 *  Practical “gotchas” you’ll avoid with this approach
 *
 *  ) Garbled Windows console output when you print UTF‑8 bytes to std::cout but the console expects
 *    a legacy codepage. [stackoverflow.com], [codegenes.net]
 *  ) Incorrect argv on Windows when you rely on char** argv and pass non-ASCII arguments. [iifx.dev], [stackoverflow.com]
 *  ) Locale-driven surprises on Unix when the terminal isn’t actually running
 *    a UTF‑8 locale (so Unicode doesn’t display or round-trip). [stackoverflow.com], [wiki.archlinux.org], [baeldung.com]
 */

/**
 * 
 *  Bottom line recommendation (assuming you want to support Unicode text in C++20):
 *
 *  Internal: UTF‑8 (std::u8string if you want C++20 type clarity; otherwise UTF‑8 std::string).[w3tutorials.net], [learn.microsoft.com]
 *
 *  Files : define as UTF‑8 always.[learn.microsoft.com], [wiki.archlinux.org]
 *
 *  Terminal :
 *      Unix   : UTF‑8 if locale is UTF‑8; otherwise treat as configuration / error.[wiki.archlinux.org], [baeldung.com]
 *      Windows: use wmain for args and wide - console strategy or explicit UTF‑8 console handling; convert at the boundary.[iifx.dev], [learn.microsoft.com], [stackoverflow.com]
 */

/*
Example usage (cross-platform entry points)

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
{
    // Bring argv into UTF-8 immediately (Windows: wmain avoids mis-decoded argv). [7](https://github.com/ww898/utf-cpp)[8](https://stackoverflow.com/questions/2527720/confused-about-cs-stdwstring-utf-16-utf-8-and-displaying-strings-in-a-win)
    auto args = text_io::argv_to_utf8(argc, argv);

    text_io::write_stdout_utf8(u8"Enter a line: ");
    text_io::text line = text_io::read_stdin_line_utf8();

    text_io::write_stdout_utf8(u8"\nYou typed: ");
    text_io::write_stdout_utf8(line);
    text_io::write_stdout_utf8(u8"\n");

    return 0;
}
#else
int main(int argc, char *argv[])
{
    auto args = text_io::argv_to_utf8(argc, argv);

    text_io::write_stdout_utf8(u8"Enter a line: ");
    text_io::text line = utf8::read_stdin_line_utf8();

    text_io::write_stdout_utf8(u8"\nYou typed: ");
    text_io::write_stdout_utf8(line);
    text_io::write_stdout_utf8(u8"\n");

    return 0;
}
#endif
*/

/*

Why this pattern works (in one screen)

Windows: You avoid the classic pitfalls:

char** argv and console byte streams are often in a non‑UTF‑8 code page, so UTF‑8 input/output can garble. [github.com], [stackoverflow.com], [cplusplus.com]
You instead accept UTF‑16 from the OS (wmain, ReadConsoleW) and output with WriteConsoleW, converting at the edge using Win32 conversion routines that explicitly support CP_UTF8. [linuxvox.com], [codegenes.net]


Unix-like: Terminals commonly operate in UTF‑8 under a UTF‑8 locale; mismatches are usually locale configuration problems. [github.com], [learnmoderncpp.com], [stackoverflow.com]
Files: You control the encoding—making them UTF‑8 removes ambiguity.

*/


namespace text_io
{

// ---------------------------- -
// Helpers: u8string <-> string
// -----------------------------
// These are *byte-preserving* reinterpretations. Use only when you know the bytes are UTF-8.
inline std::string to_string(std::u8string_view s)
{
    return std::string(reinterpret_cast<const char *>(s.data()), s.size());
}

inline std::string_view to_string_view(std::u8string_view s)
{
    return std::string_view(reinterpret_cast<const char *>(s.data()), s.size());
}

inline std::u8string to_utf8_string(std::string_view b)
{
    return std::u8string( reinterpret_cast<const char8_t *>(b.data()), b.size() );
}

inline std::u8string_view to_utf8_string_view(std::string_view b)
{
    return std::u8string_view( reinterpret_cast<const char8_t *>(b.data()), b.size() );
}

std::u8string read_line_utf8_from(std::istream &input);

inline std::u8string read_stdin_line_utf8()
{
    return read_line_utf8_from( std::cin );
}

void write_utf8_to(std::ostream &output, std::u8string_view data);

inline void write_stdout_utf8(std::u8string_view out)
{
    write_utf8_to( std::cout, out );
}

inline void write_stdout_line_utf8(std::u8string_view out)
{
    write_stdout_utf8(out);
    write_stdout_utf8( u8"\n" );
}

#if defined(_WIN32) || defined(_WIN64)
std::vector<std::u8string> argv_to_utf8(int argc, wchar_t *argv[]);
#else
std::vector<std::u8string> argv_to_utf8(int argc, char *argv[]);
#endif

}
