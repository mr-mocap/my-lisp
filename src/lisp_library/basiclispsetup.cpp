#include <my_lisp/basiclispsetup.hpp>
#include <array>

auto CommonLispSymbols = std::to_array<std::pair<StringView, SExpression>>({
    { u8"NIL",  { .value = ::Nil{}  } },
    { u8"T",    { .value = ::True{} } },
    { u8"CONS", { .value = ::Nil{}  } }
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
