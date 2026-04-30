#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <my_lisp/types/variant.hpp>


TEST_CASE("Constructors", "[SExpression]")
{
    SECTION("Default is Nil")
    {
        SExpression e;

        REQUIRE( e.type() == Variant::Type::Nil );
    }

    SECTION("NIL is Nil")
    {
        SExpression e( FundamentalType::Nil{} );

        REQUIRE( e.type() == Variant::Type::Nil );
    }

    SECTION("as True")
    {
        SExpression e( FundamentalType::True{} );

        REQUIRE( e.type() == Variant::Type::True );
    }

    SECTION("as String")
    {
        SECTION("with hard-coded string (StringArrayLike)")
        {
            SExpression e( u8"test string" );

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("with l-value StringView")
        {
            FundamentalType::StringView str_view(u8"test string");
            SExpression e( str_view );

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("with r-value StringView")
        {
            SExpression e( FundamentalType::StringView(u8"test string") );

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("copy constructed with l-value")
        {
            FundamentalType::String str(u8"test string");
            SExpression e( str );

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == str );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::String(u8"test string") );

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }
    }

    SECTION("as Pathname")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Pathname str("a/b");
            SExpression e( str );

            REQUIRE( e.type() == Variant::Type::Pathname );
            REQUIRE( e.asPathname() == str );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::Pathname("a/b") );

            REQUIRE( e.type() == Variant::Type::Pathname );
            REQUIRE( e.asPathname() == "a/b" );
        }
    }

    SECTION("as Symbol")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Symbol s{ .value = 3, .home_package = u8"COMMON-LISP" };
            SExpression e( s );

            REQUIRE( e.type() == Variant::Type::Symbol );
            REQUIRE( e.asSymbol().value == 3 );
            REQUIRE( e.asSymbol().home_package == u8"COMMON-LISP" );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::Symbol{ .value = 4, .home_package = u8"BLAH" } );

            REQUIRE( e.type() == Variant::Type::Symbol );
            REQUIRE( e.asSymbol().value == 4 );
            REQUIRE( e.asSymbol().home_package == u8"BLAH" );
        }
    }

    SECTION("as Number")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Number n = 42.4;
            SExpression e( n );

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.4) );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::Number(42.8) );

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.8) );
        }

        SECTION("copy constructed with double")
        {
            SExpression e( 42.8 );

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.8) );
        }
    }

    SECTION("as FixedNumber")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::FixedNumber n = 42;
            SExpression e( n );

            REQUIRE( e.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::FixedNumber(42.8) );

            REQUIRE( e.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }

        SECTION("copy constructed with double")
        {
            SExpression e( 42.8 );

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.8) );
        }
    }

    SECTION("as Char")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Char c = 'Q';
            SExpression e( c );

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char('Q'));
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( FundamentalType::Char('$'));

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char('$'));
        }

        SECTION("copy constructed with u8 char")
        {
            SExpression e( u8'+' );

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char( u8'+' ));
        }

        SECTION("copy constructed with U char")
        {
            SExpression e( U'猫' );

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char( U'猫' ));
        }
    }

    SECTION("as Function")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Function f = &PredefinedFunctions::atom;
            SExpression e( f );

            REQUIRE( e.type() == Variant::Type::Function );
            REQUIRE( e.asFunction() == &PredefinedFunctions::atom );
        }

        SECTION("copy constructed with function pointer")
        {
            SExpression e( &PredefinedFunctions::null );

            REQUIRE( e.type() == Variant::Type::Function );
            REQUIRE( e.asFunction() == FundamentalType::Function( &PredefinedFunctions::null ) );
        }
    }

    SECTION("as Package")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            SExpression e( p );

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( std::make_shared<Package>( u8"HOME-PACKAGE" ));

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
    }

    SECTION("as Stream")
    {
#if 0
        SECTION("copy constructed with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            SExpression e( p );

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
#endif

#if 0
        SECTION("copy constructed with r-value")
        {
            SExpression e( std::make_shared<Package>( u8"HOME-PACKAGE" ));

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
#endif
    }

#if 0
    SECTION("As ConsCell")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::ConsCellPtr c = std::make_shared<ConsCell>();

            c->car = 42;
            c->cdr = FundamentalType::Nil{};
            SExpression e( c );

            REQUIRE( e.type() == Variant::Type::ConsCell );
            REQUIRE( e.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( e.asConsCell()->cdr.type() == Variant::Type::Nil );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( std::make_shared<ConsCell>( { .car = 42, .cdr = FundamentalType::Nil{} } ) );

            REQUIRE( e.type() == Variant::Type::ConsCell );
            REQUIRE( e.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( e.asConsCell()->cdr.type() == Variant::Type::Nil );
        }
    }
#endif

    SECTION("from Bool")
    {
        SExpression e_true( true );
        SExpression e_false( false );

        REQUIRE( e_true.type() == Variant::Type::True );
        REQUIRE( e_false.type() == Variant::Type::Nil );
    }
}

TEST_CASE("Assignment", "[SExpression]")
{
    SECTION("NIL")
    {
        SExpression e( FundamentalType::Nil{} );

        REQUIRE( e.type() == Variant::Type::Nil );

        e = FundamentalType::Nil{};

        REQUIRE( e.type() == Variant::Type::Nil );
    }

    SECTION("True")
    {
        SExpression e;

        REQUIRE( e.type() == Variant::Type::Nil );

        e = FundamentalType::True{};
        
        REQUIRE( e.type() == Variant::Type::True );
    }

    SECTION("String")
    {
        SECTION("with hard-coded string (StringArrayLike)")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = u8"test string";

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("with l-value StringView")
        {
            FundamentalType::StringView str_view(u8"test string");
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = str_view;

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("with r-value String")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::String(u8"test string");

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }

        SECTION("with r-value StringView")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::StringView(u8"test string");

            REQUIRE( e.type() == Variant::Type::String );
            REQUIRE( e.asString() == u8"test string" );
        }
    }

    SECTION("as Pathname")
    {
        SECTION("with l-value")
        {
            FundamentalType::Pathname path("a/b");
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = path;

            REQUIRE( e.type() == Variant::Type::Pathname );
            REQUIRE( e.asPathname() == path );
        }

        SECTION("with r-value")
        {
            SExpression e;
            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::Pathname("a/b");

            REQUIRE( e.type() == Variant::Type::Pathname );
            REQUIRE( e.asPathname() == FundamentalType::Pathname("a/b") );
        }
    }

    SECTION("as Symbol")
    {
        SECTION(" with l-value")
        {
            FundamentalType::Symbol s{ .value = 3, .home_package = u8"COMMON-LISP" };
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );
            e = s;

            REQUIRE( e.type() == Variant::Type::Symbol );
            REQUIRE( e.asSymbol().value == 3 );
            REQUIRE( e.asSymbol().home_package == u8"COMMON-LISP" );
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::Symbol{ .value = 4, .home_package = u8"BLAH" };

            REQUIRE( e.type() == Variant::Type::Symbol );
            REQUIRE( e.asSymbol().value == 4 );
            REQUIRE( e.asSymbol().home_package == u8"BLAH" );
        }
    }

    SECTION("as Number")
    {
        SECTION("with l-value")
        {
            FundamentalType::Number n = 42.4;
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );
            REQUIRE( e.type() == Variant::Type::Nil );

            e = n;

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.4) );
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::Number(42.8);

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.8) );
        }

        SECTION("with double")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = 42.8;

            REQUIRE( e.type() == Variant::Type::Number );
            REQUIRE( e.asNumber() == FundamentalType::Number(42.8) );
        }
    }

    SECTION("as FixedNumber")
    {
        SECTION("with l-value")
        {
            FundamentalType::FixedNumber n = 42;
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = n;

            REQUIRE( e.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::FixedNumber(42);

            REQUIRE( e.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }
    }

    SECTION("as Char")
    {
        SECTION("with l-value")
        {
            FundamentalType::Char c = 'Q';
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = c;

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char('Q'));
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::Char('$');

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char('$'));
        }

        SECTION("with u8 char")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = u8'+';

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char( u8'+' ));
        }

        SECTION("with U char")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = U'猫';

            REQUIRE( e.type() == Variant::Type::Char );
            REQUIRE( e.asChar() == FundamentalType::Char( U'猫' ));
        }
    }

    SECTION("as Function")
    {
        SECTION("with l-value")
        {
            FundamentalType::Function f = &PredefinedFunctions::atom;
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = f;

            REQUIRE( e.type() == Variant::Type::Function );
            REQUIRE( e.asFunction() == &PredefinedFunctions::atom );
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = FundamentalType::Function( &PredefinedFunctions::null );

            REQUIRE( e.type() == Variant::Type::Function );
            REQUIRE( e.asFunction() == FundamentalType::Function( &PredefinedFunctions::null ) );
        }

        SECTION("with function pointer")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = &PredefinedFunctions::null;

            REQUIRE( e.type() == Variant::Type::Function );
            REQUIRE( e.asFunction() == FundamentalType::Function( &PredefinedFunctions::null ) );
        }
    }

    SECTION("as Package")
    {
        SECTION("with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = p;

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }

        SECTION("with r-value")
        {
            SExpression e;

            REQUIRE( e.type() == Variant::Type::Nil );

            e = std::make_shared<Package>(u8"HOME-PACKAGE");

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
    }

    SECTION("as Stream")
    {
#if 0
        SECTION("copy constructed with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            SExpression e( p );

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
#endif

#if 0
        SECTION("copy constructed with r-value")
        {
            SExpression e( std::make_shared<Package>( u8"HOME-PACKAGE" ));

            REQUIRE( e.type() == Variant::Type::Package );
            REQUIRE( e.asPackage()->name() == u8"HOME-PACKAGE" );
        }
#endif
    }

#if 0
    SECTION("As ConsCell")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::ConsCellPtr c = std::make_shared<ConsCell>();

            c->car = 42;
            c->cdr = FundamentalType::Nil{};
            SExpression e( c );

            REQUIRE( e.type() == Variant::Type::ConsCell );
            REQUIRE( e.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( e.asConsCell()->cdr.type() == Variant::Type::Nil );
        }

        SECTION("copy constructed with r-value")
        {
            SExpression e( std::make_shared<ConsCell>( { .car = 42, .cdr = FundamentalType::Nil{} } ) );

            REQUIRE( e.type() == Variant::Type::ConsCell );
            REQUIRE( e.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( e.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( e.asConsCell()->cdr.type() == Variant::Type::Nil );
        }
    }
#endif

    SECTION("from Bool")
    {
        SExpression e;

        REQUIRE( e.type() == Variant::Type::Nil );

        e = true;

        REQUIRE( e.type() == Variant::Type::True );

        e = false;

        REQUIRE( e.type() == Variant::Type::Nil );
    }
}
