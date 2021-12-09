// #include <Windows.h>

// Bool FILE_Open(File * file, String * filename)
// {
//     assert(file != null);

//     File f = CreateFileA(
//         filename->data,
//         GENERIC_WRITE | GENERIC_READ,
//         0,
//         NULL,
//         OPEN_ALWAYS,
//         FILE_ATTRIBUTE_NORMAL,
//         NULL
//     );

//     Bool success = f != INVALID_HANDLE_VALUE;
//     if (success)
//     {
//         *file = f;
//     }
//     return success;
// }

// Bool FILE_Size(File * file, Int64 * size)
// {
//     assert(file != null);
//     assert(*file != null);
//     assert(size != null);

//     Uint64 s = 0;
//     Bool success = GetFileSizeEx(*file, &s);
//     if (success)
//     {
//         *size = s;
//     }
//     return success;
// }

// Bool FILE_MoveCursor(File * file, Int64 amount, MoveMethod how)
// {
//     assert(file != null);
//     assert(*file != null);

//     Int32 method = FILE_BEGIN;
//     switch (how)
//     {
//     case FILE_MoveFromStart:   method = FILE_BEGIN;   break;
//     case FILE_MoveFromCurrent: method = FILE_CURRENT; break;
//     case FILE_MoveFromEnd:     method = FILE_END;     break;
//     }

//     LARGE_INTEGER i;
//     i.QuadPart = amount;

//     return SetFilePointerEx(*file, i, null, method);
// }

// Bool FILE_Read(File * file, Buffer buffer, Uint64 size)
// {
//     assert(file != null);
//     assert(*file != null);

//     Uint32 sizeRead = 0;
//     Bool success = ReadFile(*file, buffer, size, &sizeRead, NULL);

//     return success && sizeRead == size;
// }

// Bool FILE_Write(File * file, Buffer buffer, Uint64 size)
// {
//     assert(file != null);
//     assert(*file != null);

//     Uint32 sizeRead = 0;
//     Bool success = WriteFile(*file, buffer, size, &sizeRead, NULL);

//     return success && sizeRead == size;
// }

// Bool FILE_Close(File * file)
// {
//     assert(file != null);

//     if (file == null)
//     {
//         return true;
//     }

//     Bool success = CloseHandle(*file);
//     if (success)
//     {
//         *file = null;
//     }
//     return success;
// }