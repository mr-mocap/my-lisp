#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("Default constructed contains no parent Environment or current package", "[Environment]")
{
    Environment e;

    REQUIRE(!e.parent_environment());
    REQUIRE(!e.current_package());
}

TEST_CASE("Environment constructed with only a current package", "[Environment]")
{
    std::shared_ptr<Package> p = std::make_shared<Package>(u8"TestPackage");
    Environment e(p);

    REQUIRE(!e.parent_environment());
    REQUIRE(e.current_package() == p);
}

TEST_CASE("Environment constructed with a parent Environment and current package", "[Environment]")
{
    std::shared_ptr<Package> p = std::make_shared<Package>(u8"TestPackage");
    Environment parent(p);
    Environment e(&parent, p);

    REQUIRE(e.parent_environment() == &parent);
    REQUIRE(e.current_package() == p);
}
