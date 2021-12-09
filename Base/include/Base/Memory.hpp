namespace mem
{
struct Block
{
    void * data = null;
    Uint64 size = 0;

    operator bool() const
    {
        return data != null;
    }
    
    implicit Block(void * data, Uint64 size) : data(data), size(size)
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
    static SystemAllocator * instance()
    {
        static SystemAllocator instance_;
        return &instance_;
    }

    mem::Block allocate(Int64 size)
    {
        void * data = sys::allocate(size);
        if (data != null)
        {
            return mem::Block{ data, size };
        }
        return mem::Block{};
    }

    Bool grow(mem::Block & block, Int64 size)
    {
        Bool success = sys::reallocate(block.data, size);
        if (success)
        {
            block = mem::Block{ block.data, size };
        }
        return success;
    }

    void deallocate(mem::Block & block)
    {
        sys::deallocate(block.data);
        block = mem::Block{};
    }

    Bool owns(mem::Block & block)
    {
        return block.data != null;
    }
};

template <typename A, typename B>
struct FallbackAllocator : private A, private B
{
    static FallbackAllocator * instance()
    {
        static FallbackAllocator instance_;
        return &instance_;
    }

    mem::Block allocate(Int64 size)
    {
        mem::Block block = A::allocate(size);
        if (block)
        {
            return block;
        }
        return B::allocate(size);
    }

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

    Bool owns(mem::Block & block)
    {
        return A::owns(block) or B::owns(block);
    }
};

template <Uint64 T, typename A, typename B>
struct ThresholdAllocator : private A, private B
{
    static ThresholdAllocator * instance()
    {
        static ThresholdAllocator instance_;
        return &instance_;
    }

    mem::Block allocate(Int64 size)
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

    Bool grow(mem::Block & block, Int64 size)
    {
        if (block.size < T)
        {
            // NOTE@Daniel:
            //   Do not allow a block from allocator A to grow past the threshold!
            if (size >= T)
            {
                return false;
            }

            return A::grow(block, size);
        }
        else
        {
            return B::grow(block, size);
        }
    }

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

    Bool owns(mem::Block & block)
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