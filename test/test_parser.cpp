#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>
#include <print>

bool rep_line(Reader &reader, BasicLispSetup &lisp_machine)
{
    while ( true )
    {
        ParseResult result = reader.read_expression(lisp_machine.environment());

        if ( !result )
        {
            if ( result.error().kind == ParseError::EOL )
            {
                std::println();
                break;
            }

            std::println("Parse error: {} at position {}", result.error().message, result.error().position);
            return false; // Don't continue
        }
        std::print(" ");
        print( result.value(), lisp_machine.environment(), std::cout );
    }
    return true; // Continue
}

int main(void)
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    Reader reader;
    
    while ( true )
    {
        PackagePtr current_package = lisp_machine.environment().current_package();

        std::print("{} > ", text_io::to_string_view(current_package->name()));

        if ( !rep_line(reader, lisp_machine) )
            break;
    }

    return EXIT_SUCCESS;
}