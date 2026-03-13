#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>


TEST_CASE("Basic PackageCollection Usage", "[PackageCollection]")
{
    PackageCollection pc;

    SECTION("Try to find a non-existent package")
    {
        REQUIRE(!pc.find_package(u8"NonExistentPackage"));
    }
    SECTION("Adding a package")
    {
        PackagePtr p = pc.make_package(u8"TestPackage");

        REQUIRE(!pc.find_package(u8"TestPackage"));

        pc.add_package(p);

        REQUIRE(pc.find_package(u8"TestPackage"));
    }
    SECTION("Adding and then removing (by name) the same package")
    {
        PackagePtr p = pc.make_package(u8"TestPackage");

        REQUIRE(!pc.find_package(u8"TestPackage"));

        pc.add_package(p);

        REQUIRE(pc.find_package(u8"TestPackage"));
        REQUIRE(pc.find_package(u8"TestPackage") == p);

        PackagePtr old_p = pc.remove_package(u8"TestPackage");

        REQUIRE(!pc.find_package(u8"TestPackage"));
        REQUIRE(old_p == p);
    }
    SECTION("Adding and then removing (by value) the same package")
    {
        PackagePtr p = pc.make_package(u8"TestPackage");

        REQUIRE(!pc.find_package(u8"TestPackage"));

        pc.add_package(p);

        REQUIRE(pc.find_package(u8"TestPackage"));
        REQUIRE(pc.find_package(u8"TestPackage") == p);

        PackagePtr old_p = pc.remove_package(p);

        REQUIRE(!pc.find_package(u8"TestPackage"));
        REQUIRE(old_p == p);
    }
}
