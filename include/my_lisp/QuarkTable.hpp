#pragma once

#include <my_lisp/Quark.hpp>
#include <string>
#include <string_view>
#include <map>


class QuarkTable
{
    struct QuarkComparator
    {
        using is_transparent = void;// Enable heterogeneous lookup

        template<typename T, typename U>
        bool operator()(T lhs, U rhs) const noexcept
        {
            return lhs < rhs;
        }
    };

public:
    [[nodiscard]] Quark get_quark(std::string_view str) noexcept;

    [[nodiscard]] std::string_view get_string(Quark q) noexcept;

    bool contains(std::string_view key) const noexcept
    {
        return m_string_to_quark.contains(key);
    }

    bool contains(Quark q) const noexcept
    {
        for (const auto &[key, value] : m_string_to_quark) {
            if (value == q)
                return true;
        }
        return false;
    }

    bool empty() const noexcept { return m_string_to_quark.empty(); }

    std::size_t size() const noexcept
    {
        return m_string_to_quark.size();
    }

protected:
    std::map<std::string, Quark, QuarkComparator> m_string_to_quark;
};
