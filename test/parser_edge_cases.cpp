#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>

TEST_CASE("unterminated list returns NIL", "[Parser][Edge]")
{
    std::istringstream iss("(a b"); // missing closing paren
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    SExpression e = read_expression(tokenizer);

    REQUIRE(e.type() == SExpression::Nil);
}

TEST_CASE("malformed dotted pair returns NIL", "[Parser][Edge]")
{
    std::istringstream iss("(a . )"); // dot but no cdr
    Reader reader(iss);
    Tokenizer tokenizer(reader);

    SExpression e = read_expression(tokenizer);

    REQUIRE(e.type() == SExpression::Nil);
}
