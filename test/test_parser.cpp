#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>
#include <print>


int main(void)
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    Reader reader;
    
    while ( true )
    {
        PackagePtr current_package = lisp_machine.environment().current_package();

        std::print("{} >", text_io::to_string_view(current_package->name()));

        ParseResult result = reader.read_expression(lisp_machine.environment());

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