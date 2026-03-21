#include <my_lisp/basiclispsetup.hpp>
#include <my_lisp/commonlisppredefinedfunctions.hpp>
#include <array>

auto CommonLispSymbols = std::to_array<std::pair<StringView, SExpression>>({
    { u8"NIL",     { .value = ::Nil{}  } },
    { u8"T",       { .value = ::True{} } },
    { u8"NULL",    { .value = PredefinedFunctions::null  } },
    { u8"SYMBOLP", { .value = PredefinedFunctions::symbolp  } },
    { u8"ATOM",    { .value = PredefinedFunctions::atom  } },
    { u8"CONSP",   { .value = PredefinedFunctions::consp  } },
    { u8"LISTP",   { .value = PredefinedFunctions::listp  } },
    { u8"NUMBERP", { .value = PredefinedFunctions::numberp  } },
    { u8"INTEGERP",   { .value = PredefinedFunctions::integerp  } },
    { u8"RATIONALP",  { .value = PredefinedFunctions::rationalp  } },
    { u8"FLOATP",     { .value = PredefinedFunctions::floatp  } },
    { u8"REALP",      { .value = PredefinedFunctions::realp  } },
    { u8"COMPLEXP",   { .value = PredefinedFunctions::complexp  } },
    { u8"CHARACTERP", { .value = PredefinedFunctions::characterp  } },
    { u8"STRINGP",    { .value = PredefinedFunctions::stringp  } },
    { u8"FUNCTIONP",  { .value = PredefinedFunctions::functionp  } },

    { u8"NOT",     { .value = PredefinedFunctions::logical_not  } }
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
