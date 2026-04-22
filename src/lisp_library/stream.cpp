#include <my_lisp/stream.hpp>
#include <my_lisp/text_io.hpp>


bool Stream::is_input_stream() const noexcept
{
    return false;
}

bool Stream::is_output_stream() const noexcept
{
    return false;
}

InputStream  *Stream::as_input_stream()  noexcept
{
    return nullptr;
}

OutputStream *Stream::as_output_stream() noexcept
{
    return nullptr;
}

StringInputStream::StringInputStream(FundamentalType::StringView str)
    :
    m_string_stream( FundamentalType::String(str) )
{
}

void StringInputStream::close()
{
    m_closed = true;
}

void StringOutputStream::close()
{
    m_closed = true;
}

FundamentalType::String StringOutputStream::contents()
{
    FundamentalType::String contents( m_string_stream.view() );

    if ( !contents.empty() )
        m_string_stream.str( FundamentalType::String() ); // clear the stream buffer
    return contents;
}
