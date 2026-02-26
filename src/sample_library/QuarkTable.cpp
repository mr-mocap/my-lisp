#include <my_lisp/QuarkTable.hpp>


Quark QuarkTable::get_quark(std::string_view str) noexcept
{
  auto it = m_string_to_quark.find(str);

  if (it != m_string_to_quark.end())
  {
    return it->second;
  }

  Quark new_quark( static_cast<int>(m_string_to_quark.size()) );

  m_string_to_quark[ std::string(str) ] = new_quark;

  return new_quark;
}

std::string_view QuarkTable::get_string(Quark q) noexcept
{
  for (const auto &[key, value] : m_string_to_quark)
  {
    if (value == q)
      return key;
  }

  return {};
}