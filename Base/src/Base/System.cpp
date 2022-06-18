#include <Windows.h>

internal void * ModuleHandle;
internal void * ProcessHeap;

namespace sys
{
void * module_handle()
{
    return ModuleHandle;
}

void * allocate(Int64 size)
{
    return cast(void *, HeapAlloc(ProcessHeap, 0, size));
}

Bool reallocate(void * data, Int64 size)
{
    if (data == null)
    {
        return false;
    }
    return HeapReAlloc(ProcessHeap, HEAP_REALLOC_IN_PLACE_ONLY, data, size) != null;
}

void deallocate(void * ptr)
{
    HeapFree(ProcessHeap, 0, ptr);
}
}

extern Int Main();

// NOTE@Daniel:
//   Only works on link.exe
extern "C" IMAGE_DOS_HEADER __ImageBase;

Int entry_point()
{
    ModuleHandle = cast(void *, &__ImageBase);
    ProcessHeap = cast(void *, GetProcessHeap());

    return Main();
}