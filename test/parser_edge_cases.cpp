#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>


TEST_CASE("unterminated list returns an UnterminatedList error", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss("(a b"); // missing closing paren
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    auto res = read_expression(tokenizer, lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == my_lisp::ParseError::UnterminatedList);
}

TEST_CASE("malformed dotted pair returns ParseError of MalformedDottedPair", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss("(a . )"); // dot but no cdr
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    auto res2 = read_expression(tokenizer, lisp_machine.environment());

    REQUIRE(!res2.has_value());

    auto err2 = res2.error();

    REQUIRE(err2.kind == my_lisp::ParseError::MalformedDottedPair);
}

TEST_CASE("stray right parenthesis returns UnexpectedToken", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss(")");
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    auto res = read_expression(tokenizer, lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == my_lisp::ParseError::UnexpectedToken);
    REQUIRE(err.message == "Stray right parenthesis");
}

TEST_CASE("top-level unexpected token returns UnexpectedToken", "[Parser][Edge]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss(".");
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    auto res = read_expression(tokenizer, lisp_machine.environment());

    REQUIRE(!res.has_value());

    auto err = res.error();

    REQUIRE(err.kind == my_lisp::ParseError::UnexpectedToken);
    REQUIRE(err.message == "Unexpected token");
}
