#include <my_lisp/structures.hpp>
#include <my_lisp/text_io.hpp>

StringView fetch_symbol_value(Symbol s)
{
}

void print(const SExpression &expr)
{
    switch ( expr.type() )
    {
    case SExpression::Nil:
        text_io::write_stdout_utf8( u8"NIL" );
        break;
    case SExpression::String:
        {
            std::u8string_view s = std::get<std::u8string>(expr.value);

            text_io::write_stdout_utf8( u8"\"" );
            text_io::write_stdout_utf8( s );
            text_io::write_stdout_utf8( u8"\"" );
        }
        break;
    case SExpression::Symbol:
        {
            //write_stdout_utf8( sym.name );
            text_io::write_stdout_utf8( u8"NOT IMPLEMENTED" );
        }
        break;
    case SExpression::ConsCell:
        {
            text_io::write_stdout_utf8(u8"NOT IMPLEMENTED");
        }
        break;
    }
}
#if 0
                text_io::write_stdout_utf8( u8"(" );

                if ( cell->car)
                {

                }
                text_io::write_stdout_utf8( u8")" );
                print( cell->car );

                const SExpression &cdr = cell->cdr;

                std::visit(
                    Visitor{
                        [](ConsCellPtr cell)
                        {
                            write_stdout_utf8(u8" ");
                            print(cell->car);
                        }
                    },
                cdr);
                if ( !std::holds_alternative<Nil>(cdr.value) )
                {
                    write_stdout_utf8(u8" . ");
                    print(cdr);
                }
                write_stdout_utf8(u8")");
#endif

#if 0
void print(const SExpression &expr)
{
    std::visit(
        Visitor{
            [](Nil t) { write_stdout_utf8( u8"NIL" ); },
            [](ConsCellPtr cell) {
                write_stdout_utf8( u8"(" );
                print( cell->car );
                const SExpression &cdr = cell->cdr;

                while ( std::holds_alternative<ConsCellPtr>(cdr.value) )
                {
                    ConsCellPtr cell = std::get<ConsCellPtr>(cdr.value);

                    write_stdout_utf8( u8" " );
                    print( cell->car );
                    cdr = cell->cdr;
                }
                if ( !std::holds_alternative<Nil>(cdr.value) )
                {
                    write_stdout_utf8( u8" . " );
                    print( cdr );
                }
                write_stdout_utf8( u8")" );
                                 }
               }
        [](const auto &value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr ( std::is_same_v<T, Nil> )
            {
                write_stdout_utf8( u8"NIL" );
            }
            else if constexpr ( std::is_same_v<T, ConsCellPtr> )
            {
                write_stdout_utf8( u8"(" );
                print( value->car );
                SExpression cdr = value->cdr;

                while ( std::holds_alternative<ConsCellPtr>(cdr.value) )
                {
                    ConsCellPtr cell = std::get<ConsCellPtr>(cdr.value);

                    write_stdout_utf8( u8" " );
                    print( cell->car );
                    cdr = cell->cdr;
                }
                if ( !std::holds_alternative<Nil>(cdr.value) )
                {
                    write_stdout_utf8( u8" . " );
                    print( cdr );
                }
                write_stdout_utf8( u8")" );
            }
            else if constexpr ( std::is_same_v<T, String> )
            {
                write_stdout_utf8( u8"\"" );
                write_stdout_utf8( value );
                write_stdout_utf8( u8"\"" );
            }
            else if constexpr ( std::is_same_v<T, Symbol> )
            {
                write_stdout_utf8( value.name );
            }
        },
        expr.value);
}
#endif

ConsCellPtr cons(SExpression &&first, SExpression &&rest)
{
  auto cell = std::make_shared<ConsCell>( std::move(first), std::move(rest) );

  return cell;
}

ConsCellPtr cons(const SExpression &first, const SExpression &rest)
{
  auto cell = std::make_shared<ConsCell>( first, rest );

  return cell;
}

ConsCellPtr cons(SExpression &&first, const SExpression &rest)
{
  auto cell = std::make_shared<ConsCell>( std::move(first), rest );

  return cell;
}

ConsCellPtr cons(const SExpression &first, SExpression &&rest)
{
  auto cell = std::make_shared<ConsCell>( first, std::move(rest) );

  return cell;
}

