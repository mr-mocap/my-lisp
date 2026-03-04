#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <my_lisp/symboltable.hpp>


class Package
{
public:
    Package(String &&package_name) : name(std::move(package_name)) {}
    Package(StringView package_name) : name(package_name) {}
    Package(StringView package_name, SymbolTable &&sym_table)
        :
        name(package_name),
        symbol_table( std::move(sym_table) )
    {
    }

    StringView name() const noexcept { return name; }

    std::optional<Symbol> find_symbol(StringView symbol_name) const
    {
        return symbol_table.find_symbol(symbol_name);
    }

    Symbol intern(StringView symbol_name)
    {
        return symbol_table.intern(symbol_name);
    }
protected:
    String      name;
    SymbolTable symbol_table;
};
