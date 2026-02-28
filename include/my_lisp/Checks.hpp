#pragma once

#include <my_lisp/lisp_library_export.hpp>

#include <string_view>

// Normal ASCII characters
LISP_LIBRARY_EXPORT bool IsSymbolChar(char input);

// For UTF-8
LISP_LIBRARY_EXPORT bool IsSymbolChar(char8_t input);
