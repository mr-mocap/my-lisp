#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <my_lisp/basic_c_functions.hpp>


TEST_CASE("Default constructed SExpression is Nil", "[SExpression]")
{
    SExpression expr;

    REQUIRE(expr.type() == Variant::Type::Nil);
}

TEST_CASE("Creating a cons with rvalues", "[cons]")
{
    // Both String
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::String{ u8"first" } }, SExpression{ FundamentalType::String{ u8"second" } } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::Nil{} }, SExpression{ FundamentalType::Nil{} } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::Nil);
        REQUIRE(cell->car.asNil() == FundamentalType::Nil{});

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{});
    }

    // Both Symbol
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::Symbol{ 2 } }, SExpression{ FundamentalType::Symbol{ 3 } } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 2 });

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 3 });
    }

    // String, Symbol
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::String{ u8"first" } }, SExpression{ FundamentalType::Symbol{ 9 } } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 9 });
    }

    // Symbol, Nil
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::Symbol{ 5 } }, SExpression{ FundamentalType::Nil{ } } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 5 });

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{ });
    }

    // Symbol, ConsCell (Symbol, Nil)
    {
        SExpression s = basic_c_functions::cons( SExpression{ FundamentalType::Symbol{ 99 } },
                                                 SExpression{ basic_c_functions::cons( SExpression{ FundamentalType::Symbol{ 8 } },
                                                                                       SExpression{ FundamentalType::Nil{} } ) } );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

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
        SExpression s = basic_c_functions::cons( first, second );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        SExpression first{ FundamentalType::Nil{} };
        SExpression second{ FundamentalType::Nil{} };
        SExpression s = basic_c_functions::cons(first, second);

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::Nil);
        REQUIRE(cell->car.asNil() == FundamentalType::Nil{});

        REQUIRE(cell->cdr.type() == Variant::Type::Nil);
        REQUIRE(cell->cdr.asNil() == FundamentalType::Nil{});
    }

    // Both Symbol
    {
        SExpression first{ FundamentalType::Symbol{2} };
        SExpression second{ FundamentalType::Symbol{3} };
        SExpression s = basic_c_functions::cons( first, second );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::Symbol);
        REQUIRE(cell->car.asSymbol() == FundamentalType::Symbol{ 2 });
        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 3 });
    }

    // String, Symbol
    {
        SExpression first{ FundamentalType::String{ u8"first" }};
        SExpression second{ FundamentalType::Symbol{9} };
        SExpression s = basic_c_functions::cons( first, second );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

        REQUIRE(cell->car.type() == Variant::Type::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == Variant::Type::Symbol);
        REQUIRE(cell->cdr.asSymbol() == FundamentalType::Symbol{ 9 });
    }

    // Symbol, Nil
    {
        SExpression first{ FundamentalType::Symbol{5} };
        SExpression second{ FundamentalType::Nil{} };
        SExpression s = basic_c_functions::cons( first, second );

        REQUIRE(s.type() == Variant::Type::ConsCell);

        FundamentalType::ConsCellPtr cell = s.asConsCell();

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
        SExpression expr{ basic_c_functions::cons(SExpression{ FundamentalType::Symbol{ 3 } }, SExpression{ FundamentalType::Nil{} } ) };

        REQUIRE(expr.type() == Variant::Type::ConsCell);
        REQUIRE(expr.asConsCell()->car.type() == Variant::Type::Symbol);
        REQUIRE(expr.asConsCell()->car.asSymbol() == FundamentalType::Symbol{3});
        REQUIRE(expr.asConsCell()->cdr.type() == Variant::Type::Nil);
    }
}
