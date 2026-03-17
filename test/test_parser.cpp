#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>
#include <print>


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

int main(void)
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    Reader    reader;
    Tokenizer tokenizer(reader);
    
    while ( true )
    {
        std::print("{} >", text_io::to_string_view( lisp_machine.environment().current_package()->name() ));

        ParseResult result = read_expression(tokenizer, lisp_machine.environment());

        if ( result )
        {
            print( result.value(), lisp_machine.environment(), std::cout );
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