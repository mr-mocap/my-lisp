#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>

using namespace std::string_literals;

TEST_CASE("read_expression parses Symbol", "[Parser]")
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
}

TEST_CASE("read_expression parses String", "[Parser]")
{
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

TEST_CASE("read_expression parses Char", "[Parser]")
{
    SECTION("Upper-case ASCII")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#\\A");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Char);
        REQUIRE(e.asChar() == static_cast<::Char>('A'));
    }

    SECTION("Lower-case ASCII")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#\\c");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Char);
        REQUIRE(e.asChar() == static_cast<::Char>('c'));
    }
}

TEST_CASE("read_expression parses Numbers", "[Parser]")
{
    SECTION("Number with decimal in digit sequence")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("12.45");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Number);
        REQUIRE(e.asNumber() == 12.45);
    }

    SECTION("Number with decimal at start of digit sequence")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss(".45");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Number);
        REQUIRE(e.asNumber() == 0.45);
    }

    SECTION("Number with decimal at end of digit sequence")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("777.");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::Number);
        REQUIRE(e.asNumber() == 777.0);
    }

}

TEST_CASE("read_expression parses FixedNumbers", "[Parser]")
{
    SECTION("Normal Positive Integer")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("999");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 999);
    }

    SECTION("Normal Positive Integer with +")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("+999");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 999);
    }

    SECTION("FixedNumber (Negative)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("-1");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == -1);
    }

    SECTION("Binary Format (0)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#b0");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 0);
    }

    SECTION("Binary Format (1)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#b1");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 1);
    }

    SECTION("Binary Format (1111)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#b1111");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 15);
    }

    SECTION("Binary Format (1010)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#b1010");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 10);
    }

    SECTION("Binary Format (0101)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#b0101");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 5);
    }

    SECTION("Octal Format (0)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#o0");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 0);
    }

    SECTION("Octal Format (7)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#o7");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 7);
    }

    SECTION("Octal Format (10)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#o10");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 8);
    }

    SECTION("Octal Format (11)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#o11");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 9);
    }

    SECTION("Octal Format (777)")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("#o777");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::FixedNumber);
        REQUIRE(e.asFixedNumber() == 511);
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

    SECTION("List containing one item")
    {
        BasicLispSetup lisp_machine;

        lisp_machine.setup();

        std::istringstream iss("(a)");
        Reader reader(iss);

        ParseResult res = reader.read_expression(lisp_machine.environment());
        REQUIRE(res.has_value());
        SExpression e = res.value();

        REQUIRE(e.type() == SExpression::ConsCell);
        auto cell = e.asConsCellPtr();
        REQUIRE(cell != nullptr);
        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.type() == SExpression::Nil);

        REQUIRE(lisp_machine.environment().find_symbol( text_io::to_utf8_string_view("A") ).has_value());
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
