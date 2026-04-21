#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
    BasicLispSetup lisp_machine;

    lisp_machine.setup();
    Reader    reader;
    
    while ( true )
    {
        std::cout << "> " << std::flush;

        ParseResult result = reader.read_expression( lisp_machine.environment() );

        if ( result )
        {
            SExpression eval_result = PredefinedFunctions::eval( lisp_machine.environment(), result.value() );

            PredefinedFunctions::print( lisp_machine.environment(), eval_result );
        }
        else
        {
            std::cout << "Parse error: " << result.error().message <<
                         " at position " << result.error().position << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
