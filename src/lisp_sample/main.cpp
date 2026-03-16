#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>

void create_standard_packages(PackageCollection &pc)
{
    {
        PackagePtr standard_pkg = pc.make_package(u8"KEYWORD");

        pc.add_package(standard_pkg);
    }
    {
        PackagePtr cl_pkg = pc.make_package(u8"COMMON-LISP");

        pc.add_package(cl_pkg);
    }
    {
        PackagePtr clu_pkg = pc.make_package(u8"COMMON-LISP-USER");

        pc.add_package(clu_pkg);
    }
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
    PackageCollection package_collection;

    create_standard_packages(package_collection);

    Environment global_environment( package_collection.find_package(u8"COMMON-LISP-USER"),
                                    package_collection);
    Reader    reader;
    Tokenizer tokenizer(reader);
    
    while ( true )
    {
        std::cout << "> " << std::flush;

        ParseResult result = read_expression(tokenizer);

        if ( result )
        {
            print( result.value(), std::cout );
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Parse error: " << result.error().message <<
                         " at position " << result.error().position << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
