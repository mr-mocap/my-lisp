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

        std::print("{} > ", text_io::to_string_view(current_package->name()));

        ParseResult result = reader.read_expression(lisp_machine.environment());

        if ( !result )
        {
            if ( result.error().kind == ParseError::UnexpectedEOF )
            {
                std::println("Exiting...");
                break;
            }
            std::println("Parse error: {} at position {}", result.error().message, result.error().position);
            break; // Don't continue
        }

        SExpression eval_result = PredefinedFunctions::eval( lisp_machine.environment(), result.value() );

        PredefinedFunctions::print( lisp_machine.environment(), eval_result );
        std::println();
    }

    return EXIT_SUCCESS;
}