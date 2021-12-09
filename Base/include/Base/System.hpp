#pragma once

inline void * operator new(Uint64, void * where)
{
    return where;
}

inline void operator delete(void*, void*)
{
}

inline void * operator new[](Uint64, void * where)
{
    return where;
}

inline void operator delete[](void*, void*)
{
}

namespace sys
{
void * module_handle();

/**
 * @brief Allocates a block of memory via OS allocation functions
 * 
 * @param size Requested size of the block
 * @return A pointer to a newly allocated block of memory or null
 */
void * allocate(Int64 size);

/**
 * @brief Rellocates a block of memory inplace
 * 
 * @param data Block of memory that should grow
 * @param size Requested size of the block
 * @return Whether the reallocation succeeded
 */
Bool reallocate(void * data, Int64 size);

/**
 * @brief Deallocates a block of memory
 * 
 * @param data Memory to be deallocated
 */
void deallocate(void * data);
}
