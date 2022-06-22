/**
 * @file File.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-19
 *
 * @copyright Copyright (c) 2022
 *
 */

namespace io
{
struct File
{
public:
    enum Anchor
    {
        START,
        CURRENT,
        END
    };

private:
    void * handle;

public:
    File(char * filename);

    void open(char * filename);
    void close();

    i64 exact_size();
    i64 read(std::Span<byte> data, i64 offset);
    i64 write(std::Span<byte const> data, i64 offset);

    implicit File & operator=(File const & other) = delete;
    implicit File();
    implicit File(File const & other) = delete;
    implicit File(File && other);
    implicit ~File();
};
}