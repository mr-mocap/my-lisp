#include <my_lisp/symboltable.hpp>

namespace
{

int CurrentSymbolNumber = 0;

FundamentalType::Symbol GenerateUniqueSymbol()
{
    // For now, let's just increment a global counter.
    // In a later improvement, we might want to recycle symbols or use a more complex scheme.
    return FundamentalType::Symbol{ CurrentSymbolNumber++ };
}
}

FundamentalType::Symbol &SymbolTable::intern(FundamentalType::StringView str) noexcept
{
    auto it = m_string_to_symbol.find(str);

    if ( it != m_string_to_symbol.end() )
        return it->second;  // We found it!

    // Let's insert a new one...
    auto r = m_string_to_symbol.emplace( std::make_pair( FundamentalType::String(str), GenerateUniqueSymbol() ) );

    return r.first->second;
}

void SymbolTable::intern_with_no_retval(FundamentalType::StringView str) noexcept
{
    auto it = m_string_to_symbol.find(str);

    if ( it == m_string_to_symbol.end() )
        (void)m_string_to_symbol.emplace( std::make_pair( std::u8string(str), GenerateUniqueSymbol() ) );
}

void SymbolTable::unintern(FundamentalType::Symbol s) noexcept
{
    auto it = std::ranges::find_if( m_string_to_symbol,
                                    [s](const auto &pair)
                                    {
                                        return pair.second == s;
                                    });

    if ( it != m_string_to_symbol.end() )
        m_string_to_symbol.erase(it);
}

std::u8string_view SymbolTable::get_string(FundamentalType::Symbol q) const noexcept
{
    for ( const auto &[key, value] : m_string_to_symbol )
    {
        if ( value == q )
            return key;
    }

    return {};
}