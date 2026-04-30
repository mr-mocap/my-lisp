#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include "helpers.hpp"

TEST_CASE("cons creates a ConsCell", "[List]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("With NIL and True")
    {
        SExpression second = SExpression::make_cons(SExpression::make_true(), SExpression::make_nil());
        SExpression first = SExpression::make_cons( SExpression::make_nil(), second );
        SExpression stuff = build_list(SExpression{ FundamentalType::True{} }, SExpression{ FundamentalType::String{u8"test"}});
        //SExpression stuff2= build_list(FundamentalType::True{}, FundamentalType::String{u8"test"});

        REQUIRE(stuff.type() == Variant::Type::ConsCell);
        REQUIRE(stuff.asConsCell()->car.type() == Variant::Type::True);
        REQUIRE(stuff.asConsCell()->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(stuff.asConsCell()->cdr.asConsCell()->car.type() == Variant::Type::String);
        REQUIRE(stuff.asConsCell()->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);

        REQUIRE(first.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->car.type() == Variant::Type::Nil);
        REQUIRE(first.asConsCell()->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->car.type() == Variant::Type::True);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);

        SExpression cell = PredefinedFunctions::cons( lisp_machine.environment(), first );

        REQUIRE(cell.type() == Variant::Type::ConsCell);
        REQUIRE(cell.asConsCell()->car.type() == Variant::Type::Nil);
        REQUIRE(cell.asConsCell()->cdr.type() == Variant::Type::True);
    }

    SECTION("With String and Char")
    {
        SExpression second = SExpression::make_cons( FundamentalType::Char('c'), FundamentalType::Nil{});
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), second );

        REQUIRE(first.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->car.type() == Variant::Type::String);
        REQUIRE(first.asConsCell()->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->car.type() == Variant::Type::Char);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);

        SExpression cell = PredefinedFunctions::cons( lisp_machine.environment(), first );

        REQUIRE(cell.type() == Variant::Type::ConsCell);
        REQUIRE(cell.asConsCell()->car.type() == Variant::Type::String);
        REQUIRE(cell.asConsCell()->cdr.type() == Variant::Type::Char);
    }

    SECTION("With Pathname and Number")
    {
        SExpression second = SExpression::make_cons( 42.6, FundamentalType::Nil{});
        SExpression first = SExpression::make_cons( FundamentalType::Pathname("/some/file/path.cpp"), second );

        REQUIRE(first.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->car.type() == Variant::Type::Pathname);
        REQUIRE(first.asConsCell()->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->car.type() == Variant::Type::Number);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);

        SExpression cell = PredefinedFunctions::cons( lisp_machine.environment(), first );

        REQUIRE(cell.type() == Variant::Type::ConsCell);
        REQUIRE(cell.asConsCell()->car.type() == Variant::Type::Pathname);
        REQUIRE(cell.asConsCell()->cdr.type() == Variant::Type::Number);
    }

    SECTION("With FixedNumber and Number")
    {
        SExpression second = SExpression::make_cons( 42.6, FundamentalType::Nil{});
        SExpression first = SExpression::make_cons( FundamentalType::FixedNumber(36), second );

        REQUIRE(first.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->car.type() == Variant::Type::FixedNumber);
        REQUIRE(first.asConsCell()->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->car.type() == Variant::Type::Number);
        REQUIRE(first.asConsCell()->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);

        SExpression cell = PredefinedFunctions::cons( lisp_machine.environment(), first );

        REQUIRE(cell.type() == Variant::Type::ConsCell);
        REQUIRE(cell.asConsCell()->car.type() == Variant::Type::FixedNumber);
        REQUIRE(cell.asConsCell()->cdr.type() == Variant::Type::Number);
    }

    // Test for making a Package and a Function.  How do I do this?
}

TEST_CASE("first", "[List]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("Returns NIL on non-list parameters")
    {
        SECTION("NIL parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(),  SExpression::make_nil() );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("True parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(),  SExpression::make_true() );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("String parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(),  FundamentalType::String(u8"TestString"));

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Pathname parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(),  FundamentalType::Pathname("TestString"));

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Symbol parameter")
        {
            FundamentalType::Symbol symbol = lisp_machine.environment().current_package()->intern(u8"TEST-SYMBOL");
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), symbol );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Number parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), FundamentalType::Number(42) );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("FixedNumber parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), FundamentalType::FixedNumber(42) );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Char parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), FundamentalType::Char('A') );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Function parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), FundamentalType::Function(nullptr) );

            REQUIRE(result.type() == Variant::Type::Nil);
        }

        SECTION("Package parameter")
        {
            SExpression result = PredefinedFunctions::first( lisp_machine.environment(), lisp_machine.environment().current_package() );

            REQUIRE(result.type() == Variant::Type::Nil);
        }
    }

    SECTION("Returns the first element of a one-element list")
    {
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), FundamentalType::Nil{} );

        SExpression result = PredefinedFunctions::first( lisp_machine.environment(), first );

        REQUIRE(result.type() == Variant::Type::String);
        REQUIRE(result.asString() == u8"TestString");
    }

    SECTION("Returns the first element of a two-element list")
    {
        SExpression second = SExpression::make_cons( FundamentalType::Char('c'), FundamentalType::Nil{});
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), second );

        SExpression result = PredefinedFunctions::first( lisp_machine.environment(), first );

        REQUIRE(result.type() == Variant::Type::String);
        REQUIRE(result.asString() == u8"TestString");
    }
}

TEST_CASE("rest", "[List]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("Returns the rest of a one-element list")
    {
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), FundamentalType::Nil{} );

        REQUIRE(first.type() == Variant::Type::ConsCell);
        REQUIRE(first.asConsCell()->cdr.type() == Variant::Type::Nil);

        SExpression result = PredefinedFunctions::rest( lisp_machine.environment(), first );

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("Returns the rest of a two-element list")
    {
        SExpression second = SExpression::make_cons( FundamentalType::Char('c'), FundamentalType::Nil{});
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), second );

        SExpression result = PredefinedFunctions::rest( lisp_machine.environment(), first );

        REQUIRE(result.type() == Variant::Type::ConsCell);
        REQUIRE(result.asConsCell()->car.type() == Variant::Type::Char);
        REQUIRE(result.asConsCell()->car.asChar() == 'c');
        REQUIRE(result.asConsCell()->cdr.type() == Variant::Type::Nil);
    }
}
