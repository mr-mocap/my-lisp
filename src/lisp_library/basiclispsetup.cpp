#include <my_lisp/basiclispsetup.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <array>

auto CommonLispSymbols = std::to_array<std::pair<FundamentalType::StringView, SExpression>>({
    { u8"NIL",     { FundamentalType::Nil{}  } },
    { u8"T",       { FundamentalType::True{} } },
    { u8"NULL",    { PredefinedFunctions::null  } },
    { u8"SYMBOLP", { PredefinedFunctions::symbolp  } },
    { u8"ATOM",    { PredefinedFunctions::atom  } },
    { u8"CONSP",   { PredefinedFunctions::consp  } },
    { u8"LISTP",   { PredefinedFunctions::listp  } },
    { u8"NUMBERP", { PredefinedFunctions::numberp  } },
    { u8"INTEGERP",   { PredefinedFunctions::integerp  } },
    { u8"RATIONALP",  { PredefinedFunctions::rationalp  } },
    { u8"FLOATP",     { PredefinedFunctions::floatp  } },
    { u8"REALP",      { PredefinedFunctions::realp  } },
    { u8"COMPLEXP",   { PredefinedFunctions::complexp  } },
    { u8"CHARACTERP", { PredefinedFunctions::characterp  } },
    { u8"STRINGP",    { PredefinedFunctions::stringp  } },
    { u8"FUNCTIONP",  { PredefinedFunctions::functionp  } },
    { u8"PACKAGEP",   { PredefinedFunctions::packagep  } },

    { u8"EQ",      { PredefinedFunctions::eq     } },
    { u8"EQL",     { PredefinedFunctions::eql    } },
    { u8"EQUAL",   { PredefinedFunctions::equal  } },
    { u8"EQUALP",  { PredefinedFunctions::equalp } },

    { u8"NOT",     { PredefinedFunctions::logical_not  } },

    { u8"PATHNAME",  { PredefinedFunctions::pathname } },

    { u8"CONS",  { PredefinedFunctions::cons } },
    { u8"CAR",   { PredefinedFunctions::car  } },
    { u8"CDR",   { PredefinedFunctions::cdr  } },
    { u8"CAAR",  { PredefinedFunctions::caar } },
    { u8"CADR",  { PredefinedFunctions::cadr } },
    { u8"CDAR",  { PredefinedFunctions::cdar } },
    { u8"CDDR",  { PredefinedFunctions::cddr } },

    { u8"FIRST",  { PredefinedFunctions::first  } },
    { u8"REST",   { PredefinedFunctions::rest   } },
    { u8"SECOND", { PredefinedFunctions::second } },
    { u8"ENDP",   { PredefinedFunctions::endp   } },

    { u8"SETF", { PredefinedFunctions::setf } },

    { u8"PRIN1", { PredefinedFunctions::prin1 } },
    { u8"PRINT", { PredefinedFunctions::print } },

    { u8"EVAL", { PredefinedFunctions::eval } }
    });

void BasicLispSetup::create_standard_packages()
{
    {
        FundamentalType::PackagePtr standard_pkg = m_package_collection.make_package(u8"KEYWORD");

        m_package_collection.add_package(standard_pkg);
    }
    {
        FundamentalType::PackagePtr cl_pkg = m_package_collection.make_package(u8"COMMON-LISP");

        for ( const auto& [symbol_name, sexpression] : CommonLispSymbols )
        {
            FundamentalType::Symbol s( cl_pkg->intern( symbol_name ) );

            cl_pkg->export_name( symbol_name );
            cl_pkg->set_symbol_value( s, sexpression );
        }

        m_package_collection.add_package(cl_pkg);
    }
    {
        FundamentalType::PackagePtr clu_pkg = m_package_collection.make_package(u8"COMMON-LISP-USER");

        m_package_collection.add_package(clu_pkg);
        clu_pkg->use_package( u8"COMMON-LISP" );
    }
}
