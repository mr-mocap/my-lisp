#include <my_lisp/symboltable.hpp>

namespace
{
int CurrentSymbolNumber = 0;

Symbol GenerateUniqueSymbol()
{
    // For now, let's just increment a global counter.
    // In a later improvement, we might want to recycle symbols or use a more complex scheme.
    return Symbol{ CurrentSymbolNumber++ };
}
}

Symbol SymbolTable::intern(std::u8string_view str) noexcept
{
    auto it = m_string_to_symbol.find(str);

    if ( it != m_string_to_symbol.end() )
        return it->second;  // We found it!

    // Let's insert a new one...
    auto r = m_string_to_symbol.emplace( std::make_pair( std::u8string(str), GenerateUniqueSymbol() ) );

    return r.first->second;
}

void SymbolTable::unintern(Symbol s) noexcept
{
    for ( auto it = m_string_to_symbol.begin(); it != m_string_to_symbol.end(); ++it )
    {
        if ( it->second == s )
        {
            m_string_to_symbol.erase(it);
            return;
        }
    }
}

std::u8string_view SymbolTable::get_string(Symbol q) noexcept
{
    for ( const auto &[key, value] : m_string_to_symbol )
    {
        if ( value == q )
            return key;
    }

    return {};
}