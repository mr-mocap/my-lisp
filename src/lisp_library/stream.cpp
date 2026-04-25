#include <my_lisp/types/stream.hpp>
#include <my_lisp/text_io.hpp>
#include <string>


bool Stream::is_input_stream() const noexcept
{
    return false;
}

bool Stream::is_output_stream() const noexcept
{
    return false;
}

InputStream *Stream::as_input_stream()  noexcept
{
    return nullptr;
}

OutputStream *Stream::as_output_stream() noexcept
{
    return nullptr;
}

IStream::IStream(std::istream &stream)
    :
    m_stream(std::make_unique<std::istream>(stream.rdbuf()))
{}

std::optional<char8_t> IStream::read_char()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    unsigned int c = m_stream->get();

    if ( m_stream->eof() )
        return std::nullopt; // Return nullopt to indicate end of stream
    return static_cast<char8_t>(c);
}

std::optional<FundamentalType::String> IStream::read_line()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    std::string line;

    if ( std::getline(*m_stream, line) )
        return text_io::to_utf8_string(line);
    return std::nullopt; // Return nullopt to indicate end of stream
}

void IStream::close()
{
    m_closed = true;
}

OStream::OStream(std::ostream &stream)
    :
    m_stream(std::make_unique<std::ostream>(stream.rdbuf()))
{}

void OStream::close()
{
    m_closed = true;
}

void OStream::write_char(char8_t c)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    m_stream->put(c);
}

void OStream::write(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    std::string_view ascii_str = text_io::to_string_view(str);

    m_stream->write(ascii_str.data(), ascii_str.size());
}

void OStream::write_line(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    write(str);
    write_char('\n');
}

StringInputStream::StringInputStream(FundamentalType::StringView str)
    :
    m_string_stream(FundamentalType::String(str))
{}

void StringInputStream::close()
{
    m_closed = true;
}

std::optional<char8_t> StringInputStream::read_char()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    unsigned int c = m_string_stream.get();

    if ( m_string_stream.eof() )
        return std::nullopt; // Return nullopt to indicate end of stream
    return static_cast<char8_t>(c);
}

std::optional<FundamentalType::String> StringInputStream::read_line()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    FundamentalType::String line;

    if ( std::getline(m_string_stream, line) )
        return line;
    return std::nullopt; // Return nullopt to indicate end of stream
}

void StringOutputStream::close()
{
    m_closed = true;
}

FundamentalType::String StringOutputStream::contents()
{
    FundamentalType::String contents(m_string_stream.view());

    if ( !contents.empty() )
        m_string_stream.str(FundamentalType::String()); // clear the stream buffer
    return contents;
}

void StringOutputStream::write_char(char8_t c)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    m_string_stream.put(c);
}

void StringOutputStream::write(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    m_string_stream.write(str.data(), str.size());
}

void StringOutputStream::write_line(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    write(str);
    write_char('\n');
}

std::optional<char8_t> FileInputStream::read_char()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    unsigned int c = m_file_stream.get();

    if ( m_file_stream.eof() )
        return std::nullopt; // Return nullopt to indicate end of stream
    return static_cast<char8_t>(c);
}

std::optional<FundamentalType::String> FileInputStream::read_line()
{
    if ( !is_open() )
        return std::nullopt; // Return nullopt to indicate stream is closed

    std::string line;

    if ( std::getline(m_file_stream, line) )
        return text_io::to_utf8_string(line);
    return std::nullopt; // Return nullopt to indicate end of stream
}

void FileOutputStream::write_char(char8_t c)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    m_file_stream.put(c);
}

void FileOutputStream::write(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    std::string_view ascii_str = text_io::to_string_view(str);

    m_file_stream.write(ascii_str.data(), ascii_str.size());
}

void FileOutputStream::write_line(FundamentalType::StringView str)
{
    if ( !is_open() )
        return; // Do nothing if the stream is closed

    write(str);
    write_char('\n');
}
