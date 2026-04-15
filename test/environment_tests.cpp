#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("Environment constructed with only a current package", "[Environment]")
{
    PackageCollection pc; // Create an empty package collection for the Environment to reference
    PackagePtr p = pc.make_package(u8"TestPackage");

    pc.add_package( p );

    Environment e(p, pc);

    REQUIRE_FALSE(e.parent_environment());
    REQUIRE(e.current_package() == p);
    REQUIRE(e.is_global());
}

TEST_CASE("Environment constructed with a parent Environment and current package", "[Environment]")
{
    PackageCollection pc; // Create an empty package collection for the Environment to reference

    PackagePtr p = pc.make_package(u8"TestPackage");
    Environment parent(p, pc);
    Environment e = parent.make_child_environment();

    REQUIRE(e.parent_environment() == &parent);
    REQUIRE(e.current_package() == p);
    REQUIRE_FALSE(e.is_global());
    REQUIRE(parent.is_global());
}

TEST_CASE("Finding a Symbol in the global environment", "[Environment]")
{
    PackageCollection pc; // Create an empty package collection for the Environment to reference
    PackagePtr p = pc.make_package(u8"TestPackage");

    pc.add_package(p);

    Environment global_env(p, pc);

    REQUIRE(global_env.is_global());

    SECTION("finding a non-existent Symbol fails")
    {
        OptionalSymbol sym = global_env.find_symbol(u8"NonExistentSymbol");

        REQUIRE_FALSE(sym.has_value());
    }
    SECTION("finding an existing Symbol succeeds")
    {
        // Intern a symbol into the package
        FundamentalType::Symbol inserted_symbol = p->intern(u8"ExistingSymbol");
        OptionalSymbol sym = global_env.find_symbol(u8"ExistingSymbol");

        REQUIRE(sym.has_value());
        REQUIRE(sym.value() == inserted_symbol); // Assuming the first interned symbol gets value 0
    }
    SECTION("TestPackage uses AnotherPackage")
    {
        FundamentalType::PackagePtr another_package = pc.make_package(u8"AnotherPackage");

        pc.add_package(another_package);

        FundamentalType::Symbol other_symbol = another_package->intern(u8"SymbolInAnotherPackage");

        p->use_package(u8"AnotherPackage");

        SECTION("Finding a symbol from the used package fails when the other symbol is not exported")
        {
            REQUIRE_FALSE(another_package->is_exported_name(u8"SymbolInAnotherPackage"));
            REQUIRE_FALSE(global_env.find_symbol(u8"SymbolInAnotherPackage").has_value());
        }
        SECTION("Finding a symbol from the used package succeeds when the other symbol is exported")
        {
            another_package->export_name(u8"SymbolInAnotherPackage");

            REQUIRE(another_package->is_exported_name(u8"SymbolInAnotherPackage"));
            REQUIRE(global_env.find_symbol(u8"SymbolInAnotherPackage").has_value());
        }
    }
}
