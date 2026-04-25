#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <my_lisp/basic_c_functions.hpp>


TEST_CASE("list_length", "[basic_c_functions]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    // END SETUP

    SECTION("Returns 0 for NIL")
    {
        int result = basic_c_functions::list_length( SExpression::make_nil() );

        REQUIRE(result == 0);
    }

    SECTION("Returns 1 for a one-element list")
    {
        SExpression list = SExpression::make_cons( FundamentalType::String(u8"TestString"), basic_c_functions::nil() );
        int result = basic_c_functions::list_length( list );

        REQUIRE(result == 1);
    }

    SECTION("Returns 2 for a two-element list")
    {
        SExpression second = SExpression::make_cons( FundamentalType::Char('c'), basic_c_functions::nil() );
        SExpression first = SExpression::make_cons( FundamentalType::String(u8"TestString"), second );
        int result = basic_c_functions::list_length( first );

        REQUIRE(result == 2);
    }
}
