## Overview

A non-standard STL implementation only using libraries shipped with Windows.

Containers are not completely standard compliant and sometimes compliance is broken intentionally.

## Main differences

Most changes are either not commonly used, trivially replacable or are completely new features.

### Composable allocators

All allocators share a common interface:

```cpp
struct Allocator
{
    /**
     * @return A pointer to the global instance of this allocator
     */
    static Allocator * instance();

    /**
     * @param size Requested size of the allocation
     * @return An allocated block of memory
     */
    mem::Block allocate(Int64 size);

    /**
     * @brief Tries to grow a block of memory inplace
     *
     * @param block A block of memory
     * @param size The new requested size of the block
     * @return Whether the allocator was successful in growin the block
     */
    Bool grow(mem::Block & block, Int64 size);

    /**
     * @brief Destroy and invalidate a block of memory
     * 
     * @param block The block to deallocate
     */
    void deallocate(mem::Block & block);

    /**
     * @param block A block of memory
     * @return Whether the block has been allocated by this allocator
     */
    Bool owns(mem::Block & block);
};
```

All methods are mandatory, however only `Allocator::allocate()` and `Allocate::deallocate()` are required to have meaningful implementations:
 - `Allocator::instance()` may always return `null` if a global instance does not or cannot exist
 - `Allocator::grow` may always return `false` if the allocator does not support the operation
 - `Allocator::owns()` may always return `false` if the allocator cannot determine memory owndership

Allocators can be composed and nested via templates:
```cpp
template <typename A, typename B>
struct FallbackAllocator : private A, private B
{
    /**
     * @return A pointer to the global instance of this allocator
     */
    static FallbackAllocator * instance()
    {
        static FallbackAllocator instance_;
        return &instance_;
    }

    /**
     * @param size Requested size of the allocation
     * @return An allocated block of memory
     */
    mem::Block allocate(Int64 size)
    {
        mem::Block block = A::allocate(size);
        if (block)
        {
            return block;
        }
        return B::allocate(size);
    }

    /**
     * @brief Tries to grow a block of memory inplace
     *
     * @param block A block of memory
     * @param size The new requested size of the block
     * @return Whether the allocator was successful in growin the block
     */
    Bool grow(mem::Block & block, Int64 size)
    {
        if (A::owns(block))
        {
            return A::grow(block, size);
        }
        else
        {
            return B::grow(block, size);
        }
    }

    /**
     * @brief Destroy and invalidate a block of memory
     * 
     * @param block The block to deallocate
     */
    void deallocate(mem::Block & block)
    {
        if (A::owns(block))
        {
            A::deallocate(block);
        }
        else
        {
            B::deallocate(block);
        }
    }

    /**
     * @param block A block of memory
     * @return Whether the block has been allocated by this allocator
     */
    Bool owns(mem::Block & block)
    {
        return A::owns(block) or B::owns(block);
    }
};
```

### Chained iteration

Standard C++ iterators are only supported for partial backwards compatibility.
The recommended way to iterate a collection is using the `iter` preprocessor definition:
```cpp
std::Vector<int> v;
/* ... */
v.iter(int & i)
{
    i += 10;
};
```

Iterating a collection in reverse order can be done via `reverse`:
```cpp
v.reverse::iter(int & i)
{
    i += 10;
};
```

Numerous consecutive iterations can be combined and chained via `then`:
```cpp
v.iter(int & i)
{
    i += 10;
}
then::reverse::iter(int & i)
{
    i *= 10
};
```