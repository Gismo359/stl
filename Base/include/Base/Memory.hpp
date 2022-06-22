/**
 * @file Memory.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief Basic allocators
 * @version 0.1
 * @date 2021-12-09
 *
 * @copyright Copyright (c) 2021
 *
 */

namespace mem
{
struct Block
{
    void * data = null;
    u64 size = 0;

    operator bool() const
    {
        return data != null;
    }

    implicit Block(void * data, u64 size) : data(data), size(size)
    {
    }

    Block & operator=(Block const & other) = default;
    Block & operator=(Block && other) = default;

    implicit Block() = default;
    implicit Block(Block const & other) = default;
    implicit Block(Block && other) = default;
    implicit ~Block() = default;
};

struct SystemAllocator
{
    /**
     * @return A pointer to the global instance of this allocator
     */
    static SystemAllocator * instance()
    {
        static SystemAllocator instance_;
        return &instance_;
    }

    /**
     * @param size Requested size of the allocation
     * @return An allocated block of memory
     */
    mem::Block allocate(u64 size)
    {
        void * data = sys::allocate(size);
        if (data != null)
        {
            return mem::Block { data, size };
        }
        return mem::Block {};
    }

    /**
     * @brief Tries to reallocate a block of memory inplace
     *
     * @param block A block of memory
     * @param size The new requested size of the block
     * @return Whether the allocator was successful in reallocatein the block
     */
    bool reallocate(mem::Block & block, u64 size)
    {
        bool success = sys::reallocate(block.data, size);
        if (success)
        {
            block = mem::Block { block.data, size };
        }
        return success;
    }

    /**
     * @brief Destroy and invalidate a block of memory
     *
     * @param block The block to deallocate
     */
    void deallocate(mem::Block & block)
    {
        sys::deallocate(block.data);
        block = mem::Block {};
    }

    /**
     * @param block A block of memory
     * @return Whether the block has been allocated by this allocator
     */
    bool owns(mem::Block & block)
    {
        return block.data != null;
    }
};

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
    mem::Block allocate(i64 size)
    {
        mem::Block block = A::allocate(size);
        if (block)
        {
            return block;
        }
        return B::allocate(size);
    }

    /**
     * @brief Tries to reallocate a block of memory inplace
     *
     * @param block A block of memory
     * @param size The new requested size of the block
     * @return Whether the allocator was successful in reallocatein the block
     */
    bool reallocate(mem::Block & block, i64 size)
    {
        if (A::owns(block))
        {
            return A::reallocate(block, size);
        }
        else
        {
            return B::reallocate(block, size);
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
    bool owns(mem::Block & block)
    {
        return A::owns(block) or B::owns(block);
    }
};

template <u64 T, typename A, typename B>
struct ThresholdAllocator : private A, private B
{
    /**
     * @return A pointer to the global instance of this allocator
     */
    static ThresholdAllocator * instance()
    {
        static ThresholdAllocator instance_;
        return &instance_;
    }

    /**
     * @param size Requested size of the allocation
     * @return An allocated block of memory
     */
    mem::Block allocate(i64 size)
    {
        if (size < T)
        {
            return A::allocate(size);
        }
        else
        {
            return B::allocate(size);
        }
    }

    /**
     * @brief Tries to reallocate a block of memory inplace
     *
     * @param block A block of memory
     * @param size The new requested size of the block
     * @return Whether the allocator was successful in reallocatein the block
     */
    bool reallocate(mem::Block & block, i64 size)
    {
        if (block.size < T)
        {
            // NOTE@Daniel:
            //   Do not allow a block from allocator A to reallocate past the threshold!
            if (size >= T)
            {
                return false;
            }

            return A::reallocate(block, size);
        }
        else
        {
            return B::reallocate(block, size);
        }
    }

    /**
     * @brief Destroy and invalidate a block of memory
     *
     * @param block The block to deallocate
     */
    void deallocate(mem::Block & block)
    {
        if (block.size < T)
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
    bool owns(mem::Block & block)
    {
        if (block.size < T)
        {
            return A::owns(block);
        }
        else
        {
            return B::owns(block);
        }
    }
};
}