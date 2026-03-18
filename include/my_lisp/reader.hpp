#pragma once

#include <my_lisp/tokenizer.hpp>
#include <my_lisp/types/parsing.hpp>


class Environment;

class Reader
{
public:
    Reader(std::istream &input_stream = std::cin)
        :
        m_input( input_stream ),
        m_tokenizer( m_input )
    {
    }

    ParseResult read_expression(Environment &environment);
protected:
    Input     m_input;
    Tokenizer m_tokenizer;
};
