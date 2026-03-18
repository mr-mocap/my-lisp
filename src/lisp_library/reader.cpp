#include <my_lisp/reader.hpp>
#include <my_lisp/parser.hpp>


ParseResult Reader::read_expression(Environment &environment)
{
    return ::read_expression( m_tokenizer, environment );
}
