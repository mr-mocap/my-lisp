#include <my_lisp/structures.hpp>
#include <my_lisp/text_io.hpp>


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