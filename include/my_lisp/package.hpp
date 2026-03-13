#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/symboltable.hpp>
#include <my_lisp/text_io.hpp>
#include <vector>
#include <algorithm>


using OptionalSymbol = std::optional<Symbol>;

class Package
{
public:
    Package() = delete;

    // Converting Constructors
    // Also sink parameters (that's why we take them by value and move them into members)
    Package(String      package_name) : m_name( std::move(package_name) ) {}
    Package(std::string package_name) : m_name( text_io::to_utf8_string(package_name) ) {}

    Package(String package_name, SymbolTable sym_table)
        :
        m_name( std::move(package_name) ),
        m_symbol_table( std::move(sym_table) )
    {
    }
    Package(std::string package_name, SymbolTable sym_table)
        :
        m_name( text_io::to_utf8_string(package_name) ),
        m_symbol_table( std::move(sym_table) )
    {
    }

    StringView name() const noexcept { return m_name; }

    OptionalSymbol find_symbol(StringView symbol_name) const
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

    bool uses_package(StringView package_name) const
    {
        return std::ranges::find(m_uses_packages, package_name) != m_uses_packages.end();
    }

    void use_package(StringView package_name)
    {
        if ( !uses_package(package_name) )
            m_uses_packages.emplace_back( package_name );
    }

    bool is_exported_name(StringView name) const
    {
        return std::ranges::find(m_exported_names, name) != m_exported_names.end();
    }

    void export_name(StringView name)
    {
        if ( !is_exported_name(name) )
            m_exported_names.emplace_back(name);
    }

    bool is_shadowed_name(StringView name) const
    {
        return std::ranges::find(m_shadowing_names, name) != m_shadowing_names.end();
    }

    void shadow_name(StringView name)
    {
        if ( !is_shadowed_name(name) )
        {
            m_symbol_table.intern_with_no_retval(name);
            m_shadowing_names.emplace_back(name);
        }
    }

    void remove_shadowed_name(StringView name)
    {
        auto it = std::ranges::find(m_shadowing_names, name);

        if (it != m_shadowing_names.end())
            m_shadowing_names.erase(it);
    }

    const std::vector<String> &uses_packages() const
    {
        return m_uses_packages;
    }

    const std::vector<String> &exported_names() const
    {
        return m_exported_names;
    }

    const std::vector<String> &shadowing_names() const
    {
        return m_shadowing_names;
    }
protected:
    String              m_name;
    SymbolTable         m_symbol_table;
    std::vector<String> m_uses_packages;
    std::vector<String> m_exported_names;
    std::vector<String> m_shadowing_names;
};

using PackagePtr = std::shared_ptr<Package>;
