#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("Constructed Package has the given name", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(p.name() == u8"TestPackage");
}

TEST_CASE("Constructing a Package with s SymbolTable that has data passes the data into the Package", "[Package]")
{
    SymbolTable sym_table;
    Symbol s = sym_table.intern(u8"foo");

    REQUIRE( sym_table.contains(u8"foo") );
    REQUIRE( sym_table.contains(s) );

    Package p("TestPackage", sym_table);

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);
}

TEST_CASE("interning a name returns a new Symbol the first time called", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);

    Symbol s = p.intern(u8"foo");

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);

    Symbol s2 = p.intern(u8"foo");

    REQUIRE(s == s2);
}

TEST_CASE("uninterning a name removes the Symbol from the Package", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);

    Symbol s = p.intern(u8"foo");

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);

    p.unintern(s);

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);
}
