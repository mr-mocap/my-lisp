#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>

TEST_CASE("Tokenizer returns EOF token at end of input", "[Tokenizer]")
{
    std::istringstream iss("");
    Input input(iss);
    Tokenizer tokenizer(input);
    Tokenizer::Token token = tokenizer.next_token();

    REQUIRE(token.type == Tokenizer::Type_e::Eof);
}

TEST_CASE("All tokens can be read and represented", "[Tokenizer]")
{
    std::istringstream iss("");
    Input input(iss);
    Tokenizer tokenizer(input);

    SECTION("Left Parentheses")
    {
        iss.str("(");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::LeftParen);
        REQUIRE(token.text() == u8"(");
        REQUIRE(token.position() == 0);
    }

    SECTION("Right Parentheses")
    {
        iss.str(")");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::RightParen);
        REQUIRE(token.text() == u8")");
        REQUIRE(token.position() == 0);
    }

    SECTION("Symbol")
    {
        iss.str("symbol");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Symbol);
        REQUIRE(token.text() == u8"SYMBOL");
        REQUIRE(token.position() == 0);
    }

    SECTION("Number")
    {
        SECTION("Preceding decimal")
        {
            iss.str(".1");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::Number);
            REQUIRE(token.text() == u8".1");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asNumberData().value == 0.1);
        }

        SECTION("Trailing decimal")
        {
            iss.str("42.");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::Number);
            REQUIRE(token.text() == u8"42.");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asNumberData().value == 42.0);
        }

        SECTION("Decimal inside digits")
        {
            iss.str("42.0");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::Number);
            REQUIRE(token.text() == u8"42.0");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asNumberData().value == 42.0);
        }
    }

    SECTION("FixedNumber")
    {
        SECTION("No Sign returns a positive number")
        {
            iss.str("42");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"42");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == 42);
        }

        SECTION("Preceding plus returns a positive number")
        {
            iss.str("+42");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"+42");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == 42);
        }

        SECTION("Preceding minus returns a negative number")
        {
            iss.str("-42");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"-42");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == -42);
        }

        SECTION("With Binary specifier")
        {
            iss.str("#b1111");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"1111");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == 15);
        }

        SECTION("With Octal Specifier")
        {
            iss.str("#o777");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"777");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == 511);
        }

        SECTION("FixedNumber - Hex")
        {
            iss.str("#xAB");

            Tokenizer::Token token = tokenizer.next_token();

            REQUIRE(token.type == Tokenizer::Type_e::FixedNumber);
            REQUIRE(token.text() == u8"AB");
            REQUIRE(token.position() == 0);
            REQUIRE(token.asFixedNumberData().value == 171);
        }
    }

    SECTION("String")
    {
        iss.str("\"This is a string\"");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::String);
        REQUIRE(token.text() == u8"This is a string");
        REQUIRE(token.position() == 0);
    }

    SECTION("Quote")
    {
        iss.str("'");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Quote);
        REQUIRE(token.text() == u8"'");
        REQUIRE(token.position() == 0);
    }

    SECTION("Comment")
    {
        iss.str("; comment here...");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Comment);
        REQUIRE(token.text() == u8"; comment here...");
        REQUIRE(token.position() == 0);
    }

    SECTION("Dot")
    {
        iss.str(".");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Dot);
        REQUIRE(token.text() == u8".");
        REQUIRE(token.position() == 0);
    }

    SECTION("Char")
    {
        iss.str("#\\A"); // "\\" is a single backslash character in the input,
                         // not an escape sequence in the C++ string literal

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Char);
        REQUIRE(token.text() == u8"A"); // Just the character after "#\\"
        REQUIRE(token.position() == 0);
    }

    SECTION("Eof")
    {
        iss.str("");

        Tokenizer::Token token = tokenizer.next_token();

        REQUIRE(token.type == Tokenizer::Type_e::Eof);
        REQUIRE(token.text() == u8"");
        REQUIRE(token.position() == 1); // The position is AFTER the end of input (in this case, 0 characters)
    }
}

TEST_CASE("Empty list is tokenized", "[Tokenizer]")
{
    std::istringstream iss("()");
    Input input(iss);
    Tokenizer tokenizer(input);

    Tokenizer::Token token = tokenizer.next_token();

    REQUIRE(token.type == Tokenizer::Type_e::LeftParen);

    Tokenizer::Token token2 = tokenizer.next_token();

    REQUIRE(token2.type == Tokenizer::Type_e::RightParen);
}
