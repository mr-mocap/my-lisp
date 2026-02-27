#pragma once

#include <my_lisp/Symbol.hpp>
#include <string>
#include <string_view>
#include <map>


class SymbolTable
{
    struct SymbolComparator
    {
        using is_transparent = void;// Enable heterogeneous lookup

        template<typename T, typename U>
        bool operator()(T lhs, U rhs) const noexcept
        {
            return lhs < rhs;
        }
    };

public:
    [[nodiscard]] Symbol get_symbol(std::string_view str) noexcept;

    [[nodiscard]] std::string_view get_string(Symbol q) noexcept;

    bool contains(std::string_view key) const noexcept
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

    bool empty() const noexcept { return m_string_to_symbol.empty(); }

    std::size_t size() const noexcept
    {
        return m_string_to_symbol.size();
    }

protected:
    std::map<std::string, Symbol, SymbolComparator> m_string_to_symbol;
};
