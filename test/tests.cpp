#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <sstream>


TEST_CASE("Table is default constructed as empty", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
}

TEST_CASE("contains returns false when string hasn't been converted into a Symbol", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
  REQUIRE(st.contains( u8"hello" ) == false);
}

TEST_CASE("interning a string for the first time returns a new Symbol", "[SymbolTable]")
{
  SymbolTable st;

  REQUIRE(st.empty());
  REQUIRE(st.contains( u8"hello" ) == false);

  Symbol q1 = st.intern( u8"hello" );

  REQUIRE(st.size() == 1);
  REQUIRE(st.contains( u8"hello" ) == true);
}

TEST_CASE("uninterning a Symbol that exists removes it from the SymbolTable", "[SymbolTable]")
{
    SymbolTable st;

    Symbol q1 = st.intern(u8"hello");

    REQUIRE(st.size() == 1);
    REQUIRE(st.contains( u8"hello" ) == true);

    st.unintern(q1);

    REQUIRE(st.size() == 0);
    REQUIRE(st.contains( u8"hello" ) == false);
}

TEST_CASE("Default constructed SExpression is Nil", "[SExpression]")
{
    SExpression expr;

    REQUIRE(expr.type() == SExpression::Nil);
}

TEST_CASE("Creating a cons with rvalues", "[cons]")
{
    // Both String
    {
        ConsCellPtr cell = cons( SExpression{ String{ u8"first" } }, SExpression{ String{ u8"second" } } );

        REQUIRE(cell->car.type() == SExpression::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == SExpression::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        ConsCellPtr cell = cons( SExpression{ Nil{} }, SExpression{ Nil{} } );

        REQUIRE(cell->car.type() == SExpression::Nil);
        REQUIRE(cell->car.asNil() == Nil{});

        REQUIRE(cell->cdr.type() == SExpression::Nil);
        REQUIRE(cell->cdr.asNil() == Nil{});
    }

    // Both Symbol
    {
        ConsCellPtr cell = cons( SExpression{ Symbol{ 2 } }, SExpression{ Symbol{ 3 } } );

        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->car.asSymbol() == Symbol{ 2 });

        REQUIRE(cell->cdr.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.asSymbol() == Symbol{ 3 });
    }

    // String, Symbol
    {
        ConsCellPtr cell = cons( SExpression{ String{ u8"first" } }, SExpression{ Symbol{ 9 } } );

        REQUIRE(cell->car.type() == SExpression::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.asSymbol() == Symbol{ 9 });
    }

    // Symbol, Nil
    {
        ConsCellPtr cell = cons( SExpression{ Symbol{ 5 } }, SExpression{ Nil{ } } );

        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->car.asSymbol() == Symbol{ 5 });

        REQUIRE(cell->cdr.type() == SExpression::Nil);
        REQUIRE(cell->cdr.asNil() == Nil{ });
    }

    // Symbol, ConsCell (Symbol, Nil)
    {
        ConsCellPtr cell = cons( SExpression{ Symbol{ 99 } },
                                 SExpression{ cons( SExpression{ Symbol{ 8 } },
                                                    SExpression{ Nil{} } ) } );

        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->car.asSymbol() == Symbol{ 99 });

        REQUIRE(cell->cdr.type() == SExpression::ConsCell);
        REQUIRE(cell->cdr.asConsCellPtr()->car.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.asConsCellPtr()->car.asSymbol() == Symbol{8});

        REQUIRE(cell->cdr.asConsCellPtr()->cdr.type() == SExpression::Nil);
    }
}

TEST_CASE("Creating a cons with lvalues", "[cons]")
{
    // Both String
    {
        SExpression first{ String{ u8"first" } };
        SExpression second{ String{ u8"second" } };
        ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == SExpression::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == SExpression::String);
        REQUIRE(cell->cdr.asString() == u8"second");
    }

    // Both Nil
    {
        SExpression first{ Nil{} };
        SExpression second{ Nil{} };
        ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == SExpression::Nil);
        REQUIRE(cell->car.asNil() == Nil{});

        REQUIRE(cell->cdr.type() == SExpression::Nil);
        REQUIRE(cell->cdr.asNil() == Nil{});
    }

    // Both Symbol
    {
        SExpression first{ Symbol{2} };
        SExpression second{ Symbol{3} };
        ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->car.asSymbol() == Symbol{ 2 });

        REQUIRE(cell->cdr.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.asSymbol() == Symbol{ 3 });
    }

    // String, Symbol
    {
        SExpression first{ String{ u8"first" }};
        SExpression second{ Symbol{9} };
        ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == SExpression::String);
        REQUIRE(cell->car.asString() == u8"first");

        REQUIRE(cell->cdr.type() == SExpression::Symbol);
        REQUIRE(cell->cdr.asSymbol() == Symbol{ 9 });
    }

    // Symbol, Nil
    {
        SExpression first{ Symbol{5} };
        SExpression second{ Nil{} };
        ConsCellPtr cell = cons( first, second );

        REQUIRE(cell->car.type() == SExpression::Symbol);
        REQUIRE(cell->car.asSymbol() == Symbol{ 5 });

        REQUIRE(cell->cdr.type() == SExpression::Nil);
        REQUIRE(cell->cdr.asNil() == Nil{ });
    }
}

TEST_CASE("type() returns what type an SExpression is constructed with", "[SExpression]")
{
    {
        SExpression expr{ Nil() };

        REQUIRE(expr.type() == SExpression::Nil);
    }
    {
        SExpression expr{ String{ u8"some string"}};

        REQUIRE(expr.type() == SExpression::String);
        REQUIRE(expr.asString() == u8"some string");
    }
    {
        SExpression expr{ Symbol{ 3 } };

        REQUIRE(expr.type() == SExpression::Symbol);
        REQUIRE(expr.asSymbol() == Symbol{ 3 } );
    }
    {
        SExpression expr{ cons(SExpression{ Symbol{ 3 } }, SExpression{ Nil{} } ) };

        REQUIRE(expr.type() == SExpression::ConsCell);
        REQUIRE(expr.asConsCellPtr()->car.type() == SExpression::Symbol);
        REQUIRE(expr.asConsCellPtr()->car.asSymbol() == Symbol{3});

        REQUIRE(expr.asConsCellPtr()->cdr.type() == SExpression::Nil);
    }
}

TEST_CASE("print() can print a Nil SExpression", "[print]")
{
    SExpression expr{ Nil{} };
    std::ostringstream oss;

    print(expr, oss);

    REQUIRE(oss.str() == "NIL");
}

TEST_CASE("print() can print a String SExpression", "[print]")
{
    SExpression expr{ String{ u8"Hello, World!" } };
    std::ostringstream oss;

    print(expr, oss);

    REQUIRE(oss.str() == "\"Hello, World!\"");
}
#if 0
TEST_CASE("print() can print a Symbol SExpression", "[print]")
{
    SymbolTable st;
    Symbol our_symbol = st.get_symbol( u8"foo" );
    SExpression expr{ our_symbol };

    print(expr);
}
#endif
