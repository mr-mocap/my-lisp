#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("KEYWORD package exists upon construction", "[Environment]")
{
    Environment e;

    REQUIRE(e.find_package(u8"KEYWORD"));
}

TEST_CASE("", "[Environment]")
{
    Environment e;

}
