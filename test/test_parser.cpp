#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>
#include <print>

int main(void)
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();

    //Reader reader;
    
    while ( true )
    {
        PackagePtr current_package = lisp_machine.environment().current_package();

        std::print("{} > ", text_io::to_string_view(current_package->name()));

        SExpression result = PredefinedFunctions::read( lisp_machine.environment(), SExpression() );

        if ( result )
        {
            SExpression eval_result = PredefinedFunctions::eval( lisp_machine.environment(), result );

            PredefinedFunctions::print( lisp_machine.environment(), eval_result );
            std::println();
        }
    }

    return EXIT_SUCCESS;
}