#include <catch2/catch_test_macros.hpp>
#include <my_lisp/lisp_library.hpp>
#include <my_lisp/types/variant.hpp>


TEST_CASE("Constructors", "[Variant]")
{
    SECTION("Default is Nil")
    {
        Variant v;

        REQUIRE( v.type() == Variant::Type::Nil );
    }

    SECTION("NIL is Nil")
    {
        Variant v( FundamentalType::Nil{} );

        REQUIRE( v.type() == Variant::Type::Nil );
    }

    SECTION("as True")
    {
        Variant v( FundamentalType::True{} );

        REQUIRE( v.type() == Variant::Type::True );
    }

    SECTION("as String")
    {
        SECTION("with hard-coded string (StringArrayLike)")
        {
            Variant v( u8"test string" );

            REQUIRE( v.type() == Variant::Type::String );
            REQUIRE( v.asString() == u8"test string" );
        }

        SECTION("with l-value StringView")
        {
            FundamentalType::StringView str_view(u8"test string");
            Variant v( str_view );

            REQUIRE( v.type() == Variant::Type::String );
            REQUIRE( v.asString() == u8"test string" );
        }

        SECTION("with r-value StringView")
        {
            Variant v( FundamentalType::StringView(u8"test string") );

            REQUIRE( v.type() == Variant::Type::String );
            REQUIRE( v.asString() == u8"test string" );
        }

        SECTION("copy constructed with l-value")
        {
            FundamentalType::String str(u8"test string");
            Variant v( str );

            REQUIRE( v.type() == Variant::Type::String );
            REQUIRE( v.asString() == str );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::String(u8"test string") );

            REQUIRE( v.type() == Variant::Type::String );
            REQUIRE( v.asString() == u8"test string" );
        }
    }

    SECTION("as Pathname")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Pathname str("a/b");
            Variant v( str );

            REQUIRE( v.type() == Variant::Type::Pathname );
            REQUIRE( v.asPathname() == str );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::Pathname("a/b") );

            REQUIRE( v.type() == Variant::Type::Pathname );
            REQUIRE( v.asPathname() == "a/b" );
        }
    }

    SECTION("as Symbol")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Symbol s{ .value = 3, .home_package = u8"COMMON-LISP" };
            Variant v( s );

            REQUIRE( v.type() == Variant::Type::Symbol );
            REQUIRE( v.asSymbol().value == 3 );
            REQUIRE( v.asSymbol().home_package == u8"COMMON-LISP" );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::Symbol{ .value = 4, .home_package = u8"BLAH" } );

            REQUIRE( v.type() == Variant::Type::Symbol );
            REQUIRE( v.asSymbol().value == 4 );
            REQUIRE( v.asSymbol().home_package == u8"BLAH" );
        }
    }

    SECTION("as Number")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Number n = 42.4;
            Variant v( n );

            REQUIRE( v.type() == Variant::Type::Number );
            REQUIRE( v.asNumber() == FundamentalType::Number(42.4) );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::Number(42.8) );

            REQUIRE( v.type() == Variant::Type::Number );
            REQUIRE( v.asNumber() == FundamentalType::Number(42.8) );
        }

        SECTION("copy constructed with double")
        {
            Variant v( 42.8 );

            REQUIRE( v.type() == Variant::Type::Number );
            REQUIRE( v.asNumber() == FundamentalType::Number(42.8) );
        }
    }

    SECTION("as FixedNumber")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::FixedNumber n = 42;
            Variant v( n );

            REQUIRE( v.type() == Variant::Type::FixedNumber );
            REQUIRE( v.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::FixedNumber(42.8) );

            REQUIRE( v.type() == Variant::Type::FixedNumber );
            REQUIRE( v.asFixedNumber() == FundamentalType::FixedNumber(42) );
        }

        SECTION("copy constructed with double")
        {
            Variant v( 42.8 );

            REQUIRE( v.type() == Variant::Type::Number );
            REQUIRE( v.asNumber() == FundamentalType::Number(42.8) );
        }
    }

    SECTION("as Char")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Char c = 'Q';
            Variant v( c );

            REQUIRE( v.type() == Variant::Type::Char );
            REQUIRE( v.asChar() == FundamentalType::Char('Q'));
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( FundamentalType::Char('$'));

            REQUIRE( v.type() == Variant::Type::Char );
            REQUIRE( v.asChar() == FundamentalType::Char('$'));
        }

#if 0
        SECTION("copy constructed with char")
        {
            Variant v( '+' );

            REQUIRE( v.type() == Variant::Type::Char );
            REQUIRE( v.asChar() == FundamentalType::Char('+'));
        }
#endif
    }

    SECTION("as Function")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::Function f = &PredefinedFunctions::atom;
            Variant v( f );

            REQUIRE( v.type() == Variant::Type::Function );
            REQUIRE( v.asFunction() == &PredefinedFunctions::atom );
        }

        SECTION("copy constructed with function pointer")
        {
            Variant v( &PredefinedFunctions::null );

            REQUIRE( v.type() == Variant::Type::Function );
            REQUIRE( v.asFunction() == FundamentalType::Function( &PredefinedFunctions::null ) );
        }
    }

    SECTION("as Package")
    {
        SECTION("copy constructed with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            Variant v( p );

            REQUIRE( v.type() == Variant::Type::Package );
            REQUIRE( v.asPackage()->name() == u8"HOME-PACKAGE" );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( std::make_shared<Package>( u8"HOME-PACKAGE" ));

            REQUIRE( v.type() == Variant::Type::Package );
            REQUIRE( v.asPackage()->name() == u8"HOME-PACKAGE" );
        }
    }

    SECTION("as Stream")
    {
#if 0
        SECTION("copy constructed with l-value")
        {
            FundamentalType::PackagePtr p = std::make_shared<Package>( u8"HOME-PACKAGE" );
            Variant v( p );

            REQUIRE( v.type() == Variant::Type::Package );
            REQUIRE( v.asPackage()->name() == u8"HOME-PACKAGE" );
        }
#endif

#if 0
        SECTION("copy constructed with r-value")
        {
            Variant v( std::make_shared<Package>( u8"HOME-PACKAGE" ));

            REQUIRE( v.type() == Variant::Type::Package );
            REQUIRE( v.asPackage()->name() == u8"HOME-PACKAGE" );
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
            Variant v( c );

            REQUIRE( v.type() == Variant::Type::ConsCell );
            REQUIRE( v.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( v.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( v.asConsCell()->cdr.type() == Variant::Type::Nil );
        }

        SECTION("copy constructed with r-value")
        {
            Variant v( std::make_shared<ConsCell>( { .car = 42, .cdr = FundamentalType::Nil{} } ) );

            REQUIRE( v.type() == Variant::Type::ConsCell );
            REQUIRE( v.asConsCell()->car.type() == Variant::Type::FixedNumber );
            REQUIRE( v.asConsCell()->car.asFixedNumber() == 42 );
            REQUIRE( v.asConsCell()->cdr.type() == Variant::Type::Nil );
        }
    }
#endif
}

