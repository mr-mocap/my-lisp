#pragma once

#include <string>
#include <string_view>
#include <filesystem>

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

std::u8string read_stdin_line_utf8();

void write_stdout_utf8(std::u8string_view out);

void write_stdout_line_utf8(std::u8string_view out);


std::u8string read_text_file_utf8(const std::filesystem::path &filename);

void write_text_file_utf8(const std::filesystem::path &filename, std::u8string_view content);

#if defined(_WIN32) || defined(_WIN64)
std::vector<std::u8string> argv_to_utf8(int argc, wchar_t *argv[]);
#else
std::vector<std::u8string> argv_to_utf8(int argc, char *argv[]);
#endif

}
