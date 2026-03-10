#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/symboltable.hpp>
#include <my_lisp/text_io.hpp>


class Package
{
public:
    Package() = delete;
    explicit Package(String package_name) : m_name( std::move(package_name) ) {}
    explicit Package(std::string package_name) : m_name( text_io::to_utf8_string(package_name) ) {}
    Package(String package_name, SymbolTable sym_table)
        :
        m_name(package_name),
        m_symbol_table( std::move(sym_table) )
    {
    }
    Package(std::string package_name, SymbolTable sym_table)
        :
        m_name( text_io::to_utf8_string(package_name) ),
        m_symbol_table( std::move(sym_table) )
    {
    }

    Package(const Package &) = default;
    Package &operator=(const Package &) = default;

    Package(Package &&) = default;
    Package &operator=(Package &&) = default;

    StringView name() const noexcept { return m_name; }

    std::optional<Symbol> find_symbol(StringView symbol_name) const
    {
        return m_symbol_table.find_symbol(symbol_name);
    }

    Symbol intern(StringView symbol_name)
    {
        return m_symbol_table.intern(symbol_name);
    }

    void unintern(Symbol symbol)
    {
        m_symbol_table.unintern(symbol);
    }
protected:
    String      m_name;
    SymbolTable m_symbol_table;
};
