#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>


TEST_CASE("unterminated list returns an UnterminatedList error", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss("(a b"); // missing closing paren
    Reader reader(iss);

    auto res = reader.read_expression(lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == ParseError::UnterminatedList);
}

TEST_CASE("malformed dotted pair returns ParseError of MalformedDottedPair", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss("(a . )"); // dot but no cdr
    Reader reader(iss);

    auto res = reader.read_expression(lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == ParseError::MalformedDottedPair);
}

TEST_CASE("stray right parenthesis returns UnexpectedToken", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss(")");
    Reader reader(iss);

    auto res = reader.read_expression(lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == ParseError::UnexpectedToken);
    REQUIRE(err.message == "Stray right parenthesis");
}

TEST_CASE("top-level unexpected token returns UnexpectedToken", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss(".");
    Reader reader(iss);

    auto res = reader.read_expression(lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == ParseError::UnexpectedToken);
    REQUIRE(err.message == "Unexpected token");
}
