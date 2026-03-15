#include <my_lisp/lisp_library.hpp>
#include <cstdlib>
#include <iostream>


int main(void)
{
    Reader reader;
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