#pragma once

#include <my_lisp/lisp_library_export.hpp>

#include <my_lisp/symbol.hpp>
#include <my_lisp/fundamental_types.hpp>
#include <string>
#include <string_view>
#include <map>
#include <optional>


LISP_LIBRARY_EXPORT
class SymbolTable
{
    struct SymbolComparator
    {
        using is_transparent = void;// Enable heterogeneous lookup

        template<typename T, typename U>
        constexpr bool operator()(T lhs, U rhs) const noexcept
        {
            return lhs < rhs;
        }
    };

public:
    [[nodiscard]] Symbol intern(StringView str) noexcept;

    [[nodiscard]] StringView get_string(Symbol q) noexcept;

    bool contains(StringView key) const noexcept
    {
        return m_string_to_symbol.contains(key);
    }

    bool contains(Symbol q) const noexcept
    {
        for (const auto &[key, value] : m_string_to_symbol) {
            if (value == q)
                return true;
        }
        return false;
    }

    std::optional<Symbol> find_symbol(StringView str) const noexcept
    {
        auto it = m_string_to_symbol.find(str);

        if (it != m_string_to_symbol.end())
            return it->second;

        return std::nullopt;
    }

    bool empty() const noexcept { return m_string_to_symbol.empty(); }

    std::size_t size() const noexcept
    {
        return m_string_to_symbol.size();
    }

protected:
    std::map<std::u8string, Symbol, SymbolComparator> m_string_to_symbol;
};
