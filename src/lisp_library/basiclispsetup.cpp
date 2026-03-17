#include <my_lisp/basiclispsetup.hpp>
#include <array>

constexpr auto CommonLispSymbols = std::to_array<StringView>({
    u8"T",
    u8"NIL",
    u8"CONS"
    });

void BasicLispSetup::create_standard_packages()
{
    {
        PackagePtr standard_pkg = m_package_collection.make_package(u8"KEYWORD");

        m_package_collection.add_package(standard_pkg);
    }
    {
        PackagePtr cl_pkg = m_package_collection.make_package(u8"COMMON-LISP");

        for ( StringView symbol_name : CommonLispSymbols )
        {
            cl_pkg->intern( symbol_name );
            cl_pkg->export_name( symbol_name );
        }

        m_package_collection.add_package(cl_pkg);
    }
    {
        PackagePtr clu_pkg = m_package_collection.make_package(u8"COMMON-LISP-USER");

        m_package_collection.add_package(clu_pkg);
    }
}
