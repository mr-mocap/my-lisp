#include <my_lisp/basiclispsetup.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <array>

auto CommonLispSymbols = std::to_array<std::pair<StringView, SExpression>>({
    { u8"NIL",     { ::Nil{}  } },
    { u8"T",       { ::True{} } },
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

    { u8"NOT",     { PredefinedFunctions::logical_not  } }
    });

void BasicLispSetup::create_standard_packages()
{
    {
        PackagePtr standard_pkg = m_package_collection.make_package(u8"KEYWORD");

        m_package_collection.add_package(standard_pkg);
    }
    {
        PackagePtr cl_pkg = m_package_collection.make_package(u8"COMMON-LISP");

        for ( const auto& [symbol_name, sexpression] : CommonLispSymbols )
        {
            Symbol s( cl_pkg->intern( symbol_name ) );

            cl_pkg->export_name( symbol_name );
            cl_pkg->set_symbol_value( s, sexpression );
        }

        m_package_collection.add_package(cl_pkg);
    }
    {
        PackagePtr clu_pkg = m_package_collection.make_package(u8"COMMON-LISP-USER");

        m_package_collection.add_package(clu_pkg);
    }
}
