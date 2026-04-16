#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>

TEST_CASE("Pathname function tests", "[pathname]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    SECTION("pathname from Symbol")
    {
        FundamentalType::String pathname_value = u8"test/pathname.lisp";
        PackagePtr current_package = lisp_machine.environment().current_package();
        FundamentalType::Symbol s = current_package->intern(u8"pathname_symbol");

        current_package->set_symbol_value(s, SExpression{ pathname_value });

        REQUIRE(lisp_machine.environment().find_symbol_value(s) != nullptr);
        REQUIRE(lisp_machine.environment().find_symbol_value(s)->type() == Variant::Type::String);
        REQUIRE(lisp_machine.environment().find_symbol_value(s)->asString() == pathname_value);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Pathname);
        REQUIRE(result.asPathname().u8string() == pathname_value);
    }

    SECTION("pathname from String")
    {
        FundamentalType::String pathname_value = u8"test/pathname.lisp";
        SExpression s = pathname_value;

        REQUIRE(s.type() == Variant::Type::String);
        REQUIRE(s.asString() == pathname_value);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Pathname);
        REQUIRE(result.asPathname().u8string() == pathname_value);
    }


    SECTION("pathname from Pathname")
    {
        SExpression s = FundamentalType::Pathname{ u8"/some/path/name" };

        REQUIRE(s.type() == Variant::Type::Pathname);
        REQUIRE(s.asPathname().u8string() == u8"/some/path/name");

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Pathname);
        REQUIRE(result.asPathname().u8string() == u8"/some/path/name");
    }

    SECTION("pathname from Nil")
    {
        SExpression s = FundamentalType::Nil{};

        REQUIRE(s.type() == Variant::Type::Nil);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from True")
    {
        SExpression s = FundamentalType::True{};

        REQUIRE(s.type() == Variant::Type::True);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from Number")
    {
        SExpression s = FundamentalType::Number{ 42.0 };

        REQUIRE(s.type() == Variant::Type::Number);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from FixedNumber")
    {
        SExpression s = FundamentalType::FixedNumber{ 42 };

        REQUIRE(s.type() == Variant::Type::FixedNumber);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from Char")
    {
        SExpression s = FundamentalType::Char{ 'a' };

        REQUIRE(s.type() == Variant::Type::Char);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from Function")
    {
        SExpression s = FundamentalType::Function{}; // We don't actually care about the value of the function, just that it's a function type

        REQUIRE(s.type() == Variant::Type::Function);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from Package")
    {
        PackagePtr current_package = lisp_machine.environment().current_package();
        SExpression s = current_package;

        REQUIRE(s.type() == Variant::Type::Package);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("pathname from Cons")
    {
        SExpression s = cons(3.14, FundamentalType::String(u8"Hi"));

        REQUIRE(s.type() == Variant::Type::ConsCell);

        SExpression result = PredefinedFunctions::pathname(lisp_machine.environment(), s);

        REQUIRE(result.type() == Variant::Type::Nil);
    }
}
