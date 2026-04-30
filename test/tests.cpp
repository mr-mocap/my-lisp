#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
//#include <sstream>


TEST_CASE("Default constructed SExpression is Nil", "[SExpression]")
{
    SExpression expr;

    REQUIRE(expr.type() == Variant::Type::Nil);
}

TEST_CASE("Creating a cons with rvalues", "[cons]")
{
    // Both String
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::String{ u8"first" } }, SExpression{ FundamentalType::String{ u8"second" } } );

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::Nil{} }, SExpression{ FundamentalType::Nil{} } );

        REQUIRE(cell->car.type() == Variant::Type::Nil);
        REQUIRE(cell->car.asNil() == FundamentalType::Nil{});

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{});
    }

    // Both Symbol
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::Symbol{ 2 } }, SExpression{ FundamentalType::Symbol{ 3 } } );

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 2 });

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 3 });
    }

    // String, Symbol
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::String{ u8"first" } }, SExpression{ FundamentalType::Symbol{ 9 } } );

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 9 });
    }

    // Symbol, Nil
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::Symbol{ 5 } }, SExpression{ FundamentalType::Nil{ } } );
        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 5 });

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{ });
    }

    // Symbol, ConsCell (Symbol, Nil)
    {
        FundamentalType::ConsCellPtr cell = cons( SExpression{ FundamentalType::Symbol{ 99 } },
                                                  SExpression{ cons( SExpression{ FundamentalType::Symbol{ 8 } },
                                                                     SExpression{ FundamentalType::Nil{} } ) } );

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 99 });
        REQUIRE(cell->cdr.type() == Variant::Type::ConsCell);
        REQUIRE(cell->cdr.asConsCell()->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asConsCell()->car.asSymbol() == FundamentalType::Symbol{8});
        REQUIRE(cell->cdr.asConsCell()->cdr.type() == Variant::Type::Nil);
    }
}

TEST_CASE("Creating a cons with lvalues", "[cons]")
{
    // Both String
    {
        SExpression first{ FundamentalType::String{ u8"first" } };
        SExpression second{ FundamentalType::String{ u8"second" } };
        FundamentalType::ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        SExpression first{ FundamentalType::Nil{} };
        SExpression second{ FundamentalType::Nil{} };
        FundamentalType::ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == Variant::Type::Nil);
        REQUIRE(cell->car.asNil() == FundamentalType::Nil{});

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{});
    }

    // Both Symbol
    {
        SExpression first{ FundamentalType::Symbol{2} };
        SExpression second{ FundamentalType::Symbol{3} };
        FundamentalType::ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 2 });
        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 3 });
    }

    // String, Symbol
    {
        SExpression first{ FundamentalType::String{ u8"first" }};
        SExpression second{ FundamentalType::Symbol{9} };
        FundamentalType::ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 9 });
    }

    // Symbol, Nil
    {
        SExpression first{ FundamentalType::Symbol{5} };
        SExpression second{ FundamentalType::Nil{} };
        FundamentalType::ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 5 });

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{});
    }
}

TEST_CASE("type() returns what type an SExpression is constructed with", "[SExpression]")
{
    {
        SExpression expr{ FundamentalType::Nil() };

        REQUIRE(expr.type() == Variant::Type::Nil);
    }
    {
        SExpression expr{ FundamentalType::String{ u8"some string"}};

        REQUIRE(expr.type() == Variant::Type::String);
        REQUIRE(expr.asString() == u8"some string");
    }
    {
        SExpression expr{ FundamentalType::Symbol{ 3 } };

        REQUIRE(expr.type() == Variant::Type::Symbol);
        REQUIRE(expr.asSymbol() == FundamentalType::Symbol{ 3 } );
    }
    {
        SExpression expr{ cons(SExpression{ FundamentalType::Symbol{ 3 } }, SExpression{ FundamentalType::Nil{} } ) };

        REQUIRE(expr.type() == Variant::Type::ConsCell);
        REQUIRE(expr.asConsCell()->car.type() == Variant::Type::Symbol);
        REQUIRE(expr.asConsCell()->car.asSymbol() == FundamentalType::Symbol{3});
        REQUIRE(expr.asConsCell()->cdr.type() == Variant::Type::Nil);
    }
}

TEST_CASE("print() can print a Nil SExpression", "[print]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    SExpression expr{ FundamentalType::Nil{} };
    std::ostringstream oss;

    print(expr, lisp_machine.environment(), oss);

    REQUIRE(oss.str() == "NIL");
}

TEST_CASE("print() can print a String SExpression", "[print]")
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    SExpression expr{ FundamentalType::String{ u8"Hello, World!" } };
    std::ostringstream oss;

    print(expr, lisp_machine.environment(), oss);

    REQUIRE(oss.str() == "\"Hello, World!\"");
}
#if 0
TEST_CASE("print() can print a Symbol SExpression", "[print]")
{
    SymbolTable st;
    Symbol our_symbol = st.get_symbol( u8"foo" );
    SExpression expr{ FundamentalType::Symbol{ our_symbol } };

    print(expr);
}
#endif
