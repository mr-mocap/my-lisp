#pragma once

#include <my_lisp/fundamental_types.hpp>
#include <iostream>

class Reader
{
public:
    Reader()
        :
    m_input_stream( std::cin )
    {
    }

    Reader(std::istream &input_stream)
        :
    m_input_stream( input_stream )
    {
    }

    String read_line(); // Converted to UTF-8 string

    // Return true if the underlying stream has reached end-of-file.
    bool eof() const { return m_input_stream.eof(); }
protected:
    std::istream &m_input_stream;
};
