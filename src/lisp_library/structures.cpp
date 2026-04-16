#include <my_lisp/structures.hpp>
#include <my_lisp/text_io.hpp>


void print(const SExpression &expr, const Environment &environment, std::ostream &output)
{
    struct Visitor {
        explicit Visitor(std::ostream &out, const Environment &e) : output(out), env(e) {}
        explicit Visitor(std::ostream &out, const Environment &e, int i) : output(out), env(e), iteration(i) {}

        void operator()(FundamentalType::Nil) const
        {
            std::print(output, "NIL");
        }
        void operator()(FundamentalType::True) const
        {
            std::print(output, "T");
        }
        void operator()(const FundamentalType::String &s) const
        {
            std::print(output, "\"{}\"", text_io::to_string_view(s));
        }
        void operator()(const FundamentalType::Pathname &p) const
        {
            std::print(output, "\"{}\"", text_io::to_string_view(p.u8string()));
        }
        void operator()(FundamentalType::Symbol s) const
        {
            FundamentalType::StringView sym_name( env.symbol_name(s) );

            std::print(output, "{}", (sym_name.empty()) ? "NIL" : text_io::to_string_view(sym_name));
        }
        void operator()(FundamentalType::Number d) const
        {
            std::print(output, "{}", d);
        }
        void operator()(FundamentalType::FixedNumber fixednum) const
        {
            std::print(output, "{}", fixednum);
        }
        void operator()(FundamentalType::Char c) const
        {
            // naive conversion of codepoint to utf-8 bytes is not implemented;
            // print as numeric value for now
            if ( c <= 0x7F )
                std::print(output, "{}", static_cast<char>(c));
            else
                std::print(output, "Printing characters outside of ASCII range not implemented yet");
        }
        void operator()(FundamentalType::Function) const
        {
            std::print(output, "FunctionPtr");
        }
        void operator()(FundamentalType::PackagePtr) const
        {
            std::print(output, "Package");
        }
        void operator()(FundamentalType::ConsCellPtr cons) const
        {
            // Dotted pair?
            if ( cons->isDottedPair() )
            {
                std::print(output, "(");
                std::visit( Visitor( output, env ), cons->car.rawValue() );
                std::print(output, " . ");
                std::visit( Visitor( output, env ), cons->cdr.rawValue() );
                std::print(output, ")");
            }
            else if ( cons->isList() )
            {
                if ( iteration == 0 )
                    std::print(output, "(");

                std::visit( Visitor( output, env ), cons->car.rawValue() );
                std::print(output, " ");
                std::visit( Visitor( output, env, iteration + 1 ), cons->cdr.rawValue() );
            }
            else if ( cons->isEndList() )
            {
                std::visit( Visitor( output, env ), cons->car.rawValue() );
                std::print(output, ")");
            }
        }

        std::ostream &output;
        const Environment &env;
        int iteration = 0;
    };

    std::visit( Visitor( output, environment ), expr.rawValue() );
}


/**
 *  @note Use "will-move-from" parameters to avoid unnecessary copies.
 *        If the caller has an lvalue, then a copy will be made for the paramter.
 *        If the caller has an rvalue, then the move constructor will be used for the parameter.
 *        Then we just use move internally to move the parameters into the ConsCell, guaranteeing
 *        that we only make one copy/move per argument, regardless of whether the caller has an
 *        lvalue or rvalue.
 */
FundamentalType::ConsCellPtr cons(SExpression first, SExpression rest)
{
  auto cell = std::make_shared<ConsCell>( std::move(first), std::move(rest) );

  return cell;
} 