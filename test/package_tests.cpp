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
    FundamentalType::Symbol s = sym_table.intern(u8"foo");

    REQUIRE( sym_table.contains(u8"foo") );
    REQUIRE( sym_table.contains(s) );

    Package p("TestPackage", sym_table);

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);
}

TEST_CASE("interning a name returns a new Symbol the first time called", "[Package]")
{
    Package p("TestPackage");

    REQUIRE_FALSE(p.find_symbol(u8"foo").has_value());

    FundamentalType::Symbol s = p.intern(u8"foo");

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);

    FundamentalType::Symbol s2 = p.intern(u8"foo");

    REQUIRE(s == s2);
}

TEST_CASE("uninterning a name removes the Symbol from the Package", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);

    FundamentalType::Symbol s = p.intern(u8"foo");

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);

    p.unintern(s);

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);
}

TEST_CASE("importing a name/Symbol pair adds the pair to the Package", "[Package]")
{
    Package p("TestPackage");
    Package other_package("OtherPackage");
    FundamentalType::Symbol  s = other_package.intern(u8"foo");

    // Show it exists in "other_package"
    REQUIRE(other_package.find_symbol(u8"foo").has_value());
    REQUIRE(other_package.find_symbol(u8"foo").value() == s);
    REQUIRE(s.home_package == other_package.name());

    // Show it does NOT exist in "TestPackage"
    REQUIRE_FALSE(p.find_symbol(u8"foo").has_value());

    p.import(u8"foo", s);

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);
    REQUIRE(s.home_package == other_package.name()); // Didn't change the home package of the symbol
}

TEST_CASE("uses_package returns true after use_package is called with same name", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(!p.uses_package( u8"other_package" ));

    p.use_package( u8"other_package" );

    REQUIRE(p.uses_package(u8"other_package"));
}

TEST_CASE("is_exported returns true after export_symbol has been called", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(p.find_symbol(u8"foo").has_value() == false);

    FundamentalType::Symbol s = p.intern(u8"foo");

    REQUIRE(p.find_symbol(u8"foo").has_value());
    REQUIRE(p.find_symbol(u8"foo").value() == s);
    REQUIRE(!p.is_exported_name(u8"foo"));

    p.export_name(u8"foo");

    REQUIRE(p.is_exported_name(u8"foo"));
}

TEST_CASE("is_shadowed returns true after shadow_name has been called", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(!p.find_symbol(u8"foo").has_value());
    REQUIRE(!p.is_shadowed_name(u8"foo"));

    p.shadow_name(u8"foo");

    REQUIRE(p.is_shadowed_name(u8"foo"));
    REQUIRE(p.find_symbol(u8"foo").has_value());
}

TEST_CASE("remove_shadowed_name removes the name from the shadowing names", "[Package]")
{
    Package p("TestPackage");

    REQUIRE(!p.find_symbol(u8"foo").has_value());
    REQUIRE(!p.is_shadowed_name(u8"foo"));

    p.shadow_name(u8"foo");

    REQUIRE(p.is_shadowed_name(u8"foo"));
    REQUIRE(p.find_symbol(u8"foo").has_value());

    p.remove_shadowed_name(u8"foo");

    REQUIRE(!p.is_shadowed_name(u8"foo"));
    REQUIRE(p.find_symbol(u8"foo").has_value()); // It is STILL a Symbol in the Package, just not shadowed anymore
}
