#include <catch2/catch_test_macros.hpp>
#include <my_lisp/QuarkTable.hpp>


TEST_CASE("Table is default constructed as empty", "[QuarkTable]")
{
  QuarkTable qt;

  REQUIRE(qt.empty());
}

TEST_CASE("contains returns false when string hasn't been converted into a Quark", "[QuarkTable]")
{
  QuarkTable qt;

  REQUIRE(qt.empty());
  REQUIRE(qt.contains("hello") == false);
}

TEST_CASE("Getting a quark on an unseen string returns a new Quark", "[QuarkTable]")
{
  QuarkTable qt;

  REQUIRE(qt.empty());
  REQUIRE(qt.contains("hello") == false);

  Quark q1 = qt.get_quark("hello");

  REQUIRE(qt.size() == 1);
  REQUIRE(qt.contains("hello") == true);
}
