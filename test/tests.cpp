#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("Table is default constructed as empty", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
}

TEST_CASE("contains returns false when string hasn't been converted into a Symbol", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
  REQUIRE(st.contains("hello") == false);
}

TEST_CASE("Getting a quark on an unseen string returns a new Symbol", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
  REQUIRE(st.contains("hello") == false);

  Symbol q1 = st.get_symbol("hello");

  REQUIRE(st.size() == 1);
  REQUIRE(st.contains("hello") == true);
}
