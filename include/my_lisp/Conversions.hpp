#pragma once

#include <my_lisp/lisp_library_export.hpp>

#include <string>
#include <string_view>


LISP_LIBRARY_EXPORT
enum class Utf8ErrorPolicy
{
    Replace,   // insert U+FFFD and continue
    Throw,     // throw std::runtime_error
    Stop       // stop decoding and return what we have so far
};

LISP_LIBRARY_EXPORT std::u32string utf8_to_u32(std::u8string_view s, Utf8ErrorPolicy policy = Utf8ErrorPolicy::Replace);
