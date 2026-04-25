#pragma once

#include <my_lisp/types/fundamental.hpp>
#include <my_lisp/types/symbol.hpp>
#include <sstream>
#include <fstream>
#include <optional>


class InputStream;
class OutputStream;

using InputStreamPtr = std::shared_ptr<InputStream>;
using OutputStreamPtr = std::shared_ptr<OutputStream>;

using OStringStream = std::basic_ostringstream<char8_t>;
using IStringStream = std::basic_istringstream<char8_t>;


class Stream
{
public:
    Stream() = default;
    virtual ~Stream() = default;

    virtual bool is_input_stream()  const noexcept = 0;
    virtual bool is_output_stream() const noexcept = 0;

    virtual InputStream *as_input_stream()  noexcept = 0;
    virtual OutputStream *as_output_stream() noexcept = 0;

    virtual bool is_open() const noexcept = 0;
    virtual void close() = 0;
};


class SynonymStream : public Stream
{
public:
    SynonymStream(FundamentalType::Symbol symbol)
        :
        m_symbol(std::move(symbol))
    {}

    // No Copying
    SynonymStream(const SynonymStream &) = delete;
    SynonymStream &operator=(const SynonymStream &) = delete;

    // Only Moving allowed
    SynonymStream(SynonymStream &&) = default;
    SynonymStream &operator=(SynonymStream &&) = default;

    ~SynonymStream() = default;

protected:
    FundamentalType::Symbol m_symbol;
};


class InputStream : public Stream
{
public:
    virtual ~InputStream() = default;

    bool is_input_stream()  const noexcept override { return true; }

    InputStream *as_input_stream()  noexcept override { return this; }

    virtual std::optional<char8_t> read_char() = 0;
    virtual std::optional<FundamentalType::String> read_line() = 0;
};


class OutputStream : public Stream
{
public:
    virtual ~OutputStream() = default;

    bool is_output_stream() const noexcept override { return true; }

    OutputStream *as_output_stream() noexcept override { return this; }

    virtual void write_char(char8_t c) = 0;
    virtual void write(FundamentalType::StringView str) = 0;
    virtual void write_line(FundamentalType::StringView str) = 0;
};


class ConcatenatedStream : public InputStream
{
public:
    ~ConcatenatedStream() = default;

    const std::vector<InputStreamPtr> &combined_streams() const noexcept { return m_input_streams; }
protected:
    std::vector<InputStreamPtr> m_input_streams;
};


class BroadcastStream : public OutputStream
{
public:
    ~BroadcastStream() = default;

    const std::vector<OutputStreamPtr> &combined_streams() const noexcept { return m_output_streams; }
protected:
    std::vector<OutputStreamPtr> m_output_streams;
};


class TwoWayStream : public InputStream, public OutputStream
{
public:
    ~TwoWayStream() = default;

    bool is_input_stream()  const noexcept override { return InputStream::is_input_stream(); }
    bool is_output_stream() const noexcept override { return OutputStream::is_output_stream(); }

protected:
    InputStreamPtr  m_input_stream;
    OutputStreamPtr m_output_stream;
};


class EchoStream : public TwoWayStream
{
public:
    ~EchoStream() = default;

};


class IStream : public InputStream
{
public:
    IStream() = delete;
    IStream(std::istream &stream);
    ~IStream() = default;

    // No Copying allowed for string streams
    IStream(const IStream &) = delete;
    IStream &operator=(const IStream &) = delete;

    // Only moving allowed
    IStream(IStream &&) = default;
    IStream &operator=(IStream &&) = default;

    bool is_open() const noexcept override { return !m_closed; }

    void close() override;

    std::optional<char8_t> read_char() override;

    std::optional<FundamentalType::String> read_line() override;
protected:
    std::unique_ptr<std::istream> m_stream;
    bool                          m_closed = false;
};


class OStream : public OutputStream
{
public:
    OStream() = delete;
    OStream(std::ostream &stream);
    ~OStream() = default;

    // No Copying allowed for string streams
    OStream(const OStream &) = delete;
    OStream &operator=(const OStream &) = delete;

    // Only moving allowed
    OStream(OStream &&) = default;
    OStream &operator=(OStream &&) = default;

    bool is_open() const noexcept override { return !m_closed; }

    void close() override;

    void write_char(char8_t c) override;
    void write(FundamentalType::StringView str) override;
    void write_line(FundamentalType::StringView str) override;
protected:
    std::unique_ptr<std::ostream> m_stream;
    bool                          m_closed = false;
};


class StringInputStream : public InputStream
{
public:
    StringInputStream() = delete;
    StringInputStream(FundamentalType::StringView str);

    // No Copying allowed for string streams
    StringInputStream(const StringInputStream &) = delete;
    StringInputStream &operator=(const StringInputStream &) = delete;

    // Only moving allowed
    StringInputStream(StringInputStream &&) = default;
    StringInputStream &operator=(StringInputStream &&) = default;

    ~StringInputStream() = default;

    bool is_open() const noexcept override { return !m_closed; }

    void close() override;

    std::optional<char8_t> read_char() override;

    std::optional<FundamentalType::String> read_line() override;
protected:
    IStringStream m_string_stream;
    bool          m_closed = false;
};


class StringOutputStream : public OutputStream
{
public:
    StringOutputStream() = default;
    ~StringOutputStream() = default;

    bool is_open() const noexcept override { return !m_closed; }

    void close() override;

    FundamentalType::String contents();

    void write_char(char8_t c) override;
    void write(FundamentalType::StringView str) override;
    void write_line(FundamentalType::StringView str) override;
protected:
    OStringStream m_string_stream;
    bool            m_closed = false;
};


class FileInputStream : public InputStream
{
public:
    FileInputStream(const FundamentalType::Pathname &path)
        :
        m_file_stream(path)
    {}

    // No Copying allowed for file streams
    FileInputStream(const FileInputStream &) = delete;
    FileInputStream &operator=(const FileInputStream &) = delete;

    // Only moving allowed
    FileInputStream(FileInputStream &&) = default;
    FileInputStream &operator=(FileInputStream &&) = default;

    ~FileInputStream() = default;

    bool is_open() const noexcept override { return m_file_stream.is_open(); }

    void close() override { m_file_stream.close(); }

    std::optional<char8_t> read_char() override;

    std::optional<FundamentalType::String> read_line() override;
protected:
    std::ifstream m_file_stream;
};


class FileOutputStream : public OutputStream
{
public:
    FileOutputStream(const FundamentalType::Pathname &path)
        :
        m_file_stream(path)
    {}

    // No Copying allowed for file streams
    FileOutputStream(const FileOutputStream &) = delete;
    FileOutputStream &operator=(const FileOutputStream &) = delete;

    // Only moving allowed
    FileOutputStream(FileOutputStream &&) = default;
    FileOutputStream &operator=(FileOutputStream &&) = default;

    ~FileOutputStream() = default;

    bool is_open() const noexcept override { return m_file_stream.is_open(); }

    void close() override { m_file_stream.close(); }

    void write_char(char8_t c) override;
    void write(FundamentalType::StringView str) override;
    void write_line(FundamentalType::StringView str) override;
protected:
    std::ofstream m_file_stream;
};
