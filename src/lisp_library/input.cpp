#include <my_lisp/input.hpp>
#include <my_lisp/text_io.hpp>


FundamentalType::String Input::read_line()
{
    std::string line;

    std::getline(m_input_stream, line);
    return text_io::to_utf8_string(line);
}
