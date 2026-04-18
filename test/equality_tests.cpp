#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include "helpers.hpp"
#include <cassert>

TEST_CASE("eq", "[Equality]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("Identical objects")
    {
        SExpression first{ 46.0 };
        SExpression list = build_list( first, first );

        REQUIRE( list.type() == Variant::Type::ConsCell );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::Number );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asNumber() == 46.0 );

        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::Number );
        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).asNumber() == 46.0 );
        assert( first == first );

        SExpression result = PredefinedFunctions::eq( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::True);
    }
}

TEST_CASE("eql", "[Equality]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("Identical Numbers that are the same object")
    {
        SExpression first{ FundamentalType::Number{46.0} };
        SExpression list = build_list( first, first );

        assert( first == first ); // The SAME object

        SExpression result = PredefinedFunctions::eq( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::True);
    }

    SECTION("Identical FixedNumbers that are the same object")
    {
        SExpression first{ FundamentalType::FixedNumber{46}  };
        SExpression list = build_list( first, first );

        assert( first == first ); // The SAME object

        SExpression result = PredefinedFunctions::eq( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::True);
    }

    SECTION("Identical Chars that are the same object")
    {
        SExpression first{ FundamentalType::Char{'b'} };
        SExpression list = build_list( first, first );

        REQUIRE( list.type() == Variant::Type::ConsCell );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::Char );
        REQUIRE(PredefinedFunctions::first(lisp_machine.environment(), list).asChar() == FundamentalType::Char{'b'} );

        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::Char );
        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).asChar() == FundamentalType::Char{'b'} );
        assert( first == first );

        SExpression result = PredefinedFunctions::eq( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::True);
    }

    SECTION("Identical Numbers that are different objects")
    {
        SECTION("Number")
        {
            SExpression first{ 46.0 };
            SExpression second{ 46.0 };
            SExpression list = build_list( first, second );

            REQUIRE( list.type() == Variant::Type::ConsCell );
            REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::Number );
            REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asNumber() == 46.0 );

            REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::Number );
            REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).asNumber() == 46.0 );
            assert( first != second );

            SExpression result = PredefinedFunctions::eql( lisp_machine.environment(), list );

            REQUIRE(result.type() == Variant::Type::True);
        }

        SECTION("FixedNumber")
        {
            SExpression first{ FundamentalType::FixedNumber{46} };
            SExpression second{ FundamentalType::FixedNumber{46} };
            SExpression list = build_list( first, second );

            REQUIRE( list.type() == Variant::Type::ConsCell );
            REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::FixedNumber );
            REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asFixedNumber() == 46 );

            REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::FixedNumber );
            REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).asFixedNumber() == 46 );
            assert( first != second );

            SExpression result = PredefinedFunctions::eql( lisp_machine.environment(), list );

            REQUIRE(result.type() == Variant::Type::True);
        }
    }

    SECTION("Identical Numbers that are different types of objects")
    {
        SExpression first{ FundamentalType::Number{46.0} };
        SExpression second{ FundamentalType::FixedNumber{46} };
        SExpression list = build_list( first, second );

        REQUIRE( list.type() == Variant::Type::ConsCell );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::Number );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asNumber() == 46.0 );

        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::FixedNumber );
        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).asFixedNumber() == 46 );
        assert( first != second );

        SExpression result = PredefinedFunctions::eql( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::Nil);
    }

    SECTION("Identical Chars that are different objects")
    {
        SExpression first{ FundamentalType::Char{'a'} };
        SExpression second{ FundamentalType::Char{'a'} };
        SExpression list = build_list( first, second );

        REQUIRE( list.type() == Variant::Type::ConsCell );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::Char );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asChar() == FundamentalType::Char{'a'} );

        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::Char );
        REQUIRE(PredefinedFunctions::second(lisp_machine.environment(), list).asChar() == FundamentalType::Char{ 'a' } );
        assert( first != second );

        SExpression result = PredefinedFunctions::eql( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::True);
    }

    SECTION("Identical Strings that are different objects")
    {
        SExpression first{ u8"TestString" };
        SExpression second{ u8"TestString" };
        SExpression list = build_list( first, second );

        REQUIRE( list.type() == Variant::Type::ConsCell );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).type() == Variant::Type::String );
        REQUIRE( PredefinedFunctions::first( lisp_machine.environment(), list ).asString() == FundamentalType::String{u8"TestString"} );

        REQUIRE( PredefinedFunctions::second( lisp_machine.environment(), list ).type() == Variant::Type::String );
        REQUIRE(PredefinedFunctions::second(lisp_machine.environment(), list).asString() == FundamentalType::String{ u8"TestString" } );
        assert( first != second );

        SExpression result = PredefinedFunctions::eql( lisp_machine.environment(), list );

        REQUIRE(result.type() == Variant::Type::Nil);
    }
}
