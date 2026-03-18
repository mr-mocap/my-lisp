#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>

using namespace std::string_literals;

TEST_CASE("read_expression parses symbols and strings", "[Parser]")
{
    SECTION("Symbol")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("symbol");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Symbol);
    }

    SECTION("String")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("\"hello\"");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::String);
        REQUIRE(e.asString() == u8"hello");
    }
}

TEST_CASE("read_expression parses lists and dotted pairs", "[Parser]")
{
    SECTION("Empty list")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("()");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Nil);
    }

    SECTION("Simple list")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("(a b)");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::ConsCell);
        auto cell = e.asConsCellPtr();
        REQUIRE(cell != nullptr);
        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.type() == SExpression::ConsCell);

        auto cell2 = cell->cdr.asConsCellPtr();
        REQUIRE(cell2->car.type() == SExpression::Symbol);
        REQUIRE(cell2->cdr.type() == SExpression::Nil);
    }

    SECTION("Dotted pair")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("(a . b)");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::ConsCell);
        auto cell = e.asConsCellPtr();
        REQUIRE(cell->car.type() == SExpression::Symbol);
        // cdr should be a symbol 'b'
        REQUIRE(cell->cdr.type() == SExpression::Symbol);
    }

    SECTION("Nested list")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("(a (b c))");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::ConsCell);

        auto cell = e.asConsCellPtr();

        // Symbol for "a"
        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.type() == SExpression::ConsCell); // next cell for "(b c)"

        auto rest = cell->cdr.asConsCellPtr();

        REQUIRE(rest->car.type() == SExpression::ConsCell); // "(b c)"
        REQUIRE(rest->cdr.type() == SExpression::Nil);

        auto inner = rest->car.asConsCellPtr();

        REQUIRE(inner->car.type() == SExpression::Symbol); // "b"
        REQUIRE(inner->cdr.type() == SExpression::ConsCell);

        auto inner_rest = inner->cdr.asConsCellPtr();

        REQUIRE(inner_rest->car.type() == SExpression::Symbol); // "c"
        REQUIRE(inner_rest->cdr.type() == SExpression::Nil);
    }
}

TEST_CASE("read_expression handles quote", "[Parser]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    std::istringstream iss("'x");
    Reader reader(iss);

    ParseResult res = reader.read_expression(lisp_machine.environment());

    REQUIRE(res.has_value());

    SExpression e = res.value();

    REQUIRE(e.type() == SExpression::ConsCell);

    ConsCellPtr cell = e.asConsCellPtr();

    REQUIRE(cell->car.type() == SExpression::Symbol); // quote symbol
    REQUIRE(cell->cdr.type() == SExpression::ConsCell);

    ConsCellPtr tail = cell->cdr.asConsCellPtr();

    REQUIRE(tail->car.type() == SExpression::Symbol); // quoted symbol x
    REQUIRE(tail->cdr.type() == SExpression::Nil);
}
