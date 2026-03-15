#include <my_lisp/structures.hpp>
#include <my_lisp/text_io.hpp>
#include <format>


void print(const SExpression &expr, std::ostream &output)
{
    struct Visitor {
        explicit Visitor(std::ostream &out) : output(out) {}

        void operator()(Nil) const
        {
            output << "NIL";
        }
        void operator()(const String &s) const
        {
            output << std::format("\"{}\"", text_io::to_string_view(s));
        }
        void operator()(double d) const
        {
            // simple number printing
            std::string s = std::to_string(d);

            output << s;
        }
        void operator()(bool b) const
        {
            output << (b) ? "#t" : "#f";
        }
        void operator()(char32_t c) const
        {
            // naive conversion of codepoint to utf-8 bytes is not implemented;
            // print as numeric value for now
            output << static_cast<int>(c);
        }
        void operator()(Symbol) const
        {
            output << "NOT IMPLEMENTED";
        }
        void operator()(const ConsCellPtr &) const
        {
            output << "NOT IMPLEMENTED";
        }

        std::ostream &output;
    };

    std::visit( Visitor( output ), expr.value );
}


/**
 *  @note Use "will-move-from" parameters to avoid unnecessary copies.
 *        If the caller has an lvalue, then a copy will be made for the paramter.
 *        If the caller has an rvalue, then the move constructor will be used for the parameter.
 *        Then we just use move internally to move the parameters into the ConsCell, guaranteeing
 *        that we only make one copy/move per argument, regardless of whether the caller has an
 *        lvalue or rvalue.
 */
ConsCellPtr cons(SExpression first, SExpression rest)
{
  auto cell = std::make_shared<ConsCell>( std::move(first), std::move(rest) );

  return cell;
} 