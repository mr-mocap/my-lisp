#include <my_lisp/SymbolTable.hpp>


Symbol SymbolTable::get_symbol(std::string_view str) noexcept
{
  auto it = m_string_to_symbol.find(str);

  if (it != m_string_to_symbol.end())
  {
    return it->second;
  }

  Symbol new_quark( static_cast<int>(m_string_to_symbol.size()) );

  m_string_to_symbol[ std::string(str) ] = new_quark;

  return new_quark;
}

std::string_view SymbolTable::get_string(Symbol q) noexcept
{
  for (const auto &[key, value] : m_string_to_symbol)
  {
    if (value == q)
      return key;
  }

  return {};
}