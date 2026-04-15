#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <iostream>

class Input
{
public:
    Input()
        :
    m_input_stream( std::cin )
    {
    }

    Input(std::istream &input_stream)
        :
    m_input_stream( input_stream )
    {
    }

    FundamentalType::String read_line(); // Converted to UTF-8 string

    // Return true if the underlying stream has reached end-of-file.
    bool eof() const { return m_input_stream.eof(); }
protected:
    std::istream &m_input_stream;
};
