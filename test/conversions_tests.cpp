#include <catch2/catch_test_macros.hpp>

#include <my_lisp/lisp_library.hpp>


TEST_CASE("Empty u8 string does not convert to u32 code point", "[Conversions]")
{
    REQUIRE( utf8_to_u32( u8"" ).empty() );
}

TEST_CASE("ASCII values in u8 string are converted to u32", "[Conversions]")
{
    {
        std::u32string result = utf8_to_u32( u8"A" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'A' );
    }
    {
        std::u32string result = utf8_to_u32( u8"B" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'B' );
    }
    {
        std::u32string result = utf8_to_u32( u8"Z" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'Z' );
    }
    {
        std::u32string result = utf8_to_u32( u8"a" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'a' );
    }
    {
        std::u32string result = utf8_to_u32( u8"b" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'b' );
    }
    {
        std::u32string result = utf8_to_u32( u8"z" );

        REQUIRE( result.size() == 1 );
        REQUIRE( result[0] == U'z' );
    }
}
