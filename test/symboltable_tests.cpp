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
    REQUIRE(st.contains(u8"hello") == false);
}

TEST_CASE("interning a string for the first time returns a new Symbol", "[SymbolTable]")
{
    SymbolTable st;

    REQUIRE(st.empty());
    REQUIRE(st.contains(u8"hello") == false);

    Symbol q1 = st.intern(u8"hello");

    REQUIRE(st.size() == 1);
    REQUIRE(st.contains(u8"hello") == true);
}

TEST_CASE("uninterning a Symbol that exists removes it from the SymbolTable", "[SymbolTable]")
{
    SymbolTable st;

    Symbol q1 = st.intern(u8"hello");

    REQUIRE(st.size() == 1);
    REQUIRE(st.contains( u8"hello" ) == true);

    st.unintern(q1);

    REQUIRE(st.size() == 0);
    REQUIRE(st.contains( u8"hello" ) == false);
}
