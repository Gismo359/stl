/**
 * @file File.cpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Windows.h>

namespace io
{
void File::open(char * filename)
{
    assert(filename != null);

    HANDLE result = CreateFileA(
        filename,
        GENERIC_WRITE | GENERIC_READ,
        0,
        null,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        null
    );

    if (result != INVALID_HANDLE_VALUE)
    {
        handle = result;
    }
}

i64 File::exact_size()
{
    i64 size;
    if (GetFileSizeEx(handle, cast(PLARGE_INTEGER, &size)))
    {
        return size;
    }
    return -1;
}

i64 File::read(std::Span<byte> data, i64 offset)
{
    if (data.empty())
    {
        return 0;
    }

    OVERLAPPED ov = { 0 };
    ov.Pointer = cast(void *, offset);

    i64 size = 0;
    if (ReadFile(handle, data.data(), data.size(), null, &ov))
    {
        return size;
    }
    return -1;
}

i64 File::write(std::Span<byte const> data, i64 offset)
{
    if (data.empty())
    {
        return 0;
    }

    OVERLAPPED ov = { 0 };
    ov.Pointer = cast(void *, offset);

    i64 size = 0;
    if (WriteFile(handle, data.data(), data.size(), null, &ov))
    {
        return size;
    }
    return -1;
}

File::~File()
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
    }
}
}