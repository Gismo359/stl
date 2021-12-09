/**
 * @file Vector.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief A dynamic array of objects
 * @version 0.1
 * @date 2021-12-07
 *
 * @copyright Copyright (c) 2021
 *
 */

namespace std
{
/**
 * @brief A dynamic array of objects
 *
 * @tparam T Type of underlying objects
 * @tparam A Type of allocator to use during allocation
 */
template <typename T, typename A = mem::SystemAllocator>
struct Vector : Span<T>
{
private:
    using Base = Span<T>;
    using This = Vector<T, A>;

protected:
    A * allocator_ = null;
    Uint64 capacity_ = 0;

    /**
     * @brief Internal setter for Vector::capacity()
     *
     * @param capacity New capacity
     */
    inline void set_capacity(Uint64 capacity)
    {
        capacity_ = capacity;
    }

    /**
     * @brief Reserves and sets Vector::size(), but does not (de)initialize invalidated objects
     *
     * @param size
     */
    inline void unsafe_resize(Uint64 size)
    {
        reserve(size);
        set_size(size);
    }

    /**
     * @brief
     *
     * @param capacity
     * @return Bool
     */
    inline Bool reallocate(Uint64 capacity)
    {
        mem::Block block{ data(), capacity_ * sizeof(T) };
        if (allocator().grow(block, capacity * sizeof(T)))
        {
            set_capacity(capacity);
            return true;
        }
        return false;
    }

    inline void deallocate()
    {
        mem::Block block{ data(), capacity_ * sizeof(T) };
        allocator().deallocate(block);
    }

public:
    /**
     * @return This instance's allocator
     */
    inline A const & allocator() const
    {
        return *allocator_;
    }

    /**
     * @return This instance's allocator
     */
    inline A & allocator()
    {
        return *allocator_;
    }

    /**
     * @return Number of elements this vector can hold before reallocating
     */
    inline Uint64 capacity() const
    {
        return capacity_;
    }

    inline void reserve(Uint64 capacity)
    {
        if (capacity <= This::capacity())
        {
            return;
        }

        if (reallocate(capacity))
        {
            return;
        }

        mem::Block block = allocator().allocate(capacity * sizeof(T));
        assert(block);

        Base span{ cast(Pointer, block.data), size() };
        util::raw_move_range(data(), span.data(), size());
        deallocate();

        set_data(span.data());
        set_capacity(capacity);
    }

    inline void resize(Uint64 size, T const & value = T{})
    {
        Uint64 current_size = This::size();
        if (size <= current_size)
        {
            for (auto & item : right(size))
            {
                item.~T();
            }
            set_size(size);
        }
        else
        {
            Uint64 old = current_size;
            unsafe_resize(size);
            for (auto & item : right(old))
            {
                new (&item) T(value);
            }
        }
    }

    /**
     * @brief Inserts an object at the end of the array
     *
     * @param item Object to insert
     */
    inline void push_back(T item)
    {
        resize(size() + 1, std::move(item));
    }

    /**
     * @brief Creates an object at the end of the array
     *
     * @param args Arguments, forwarded to the new object's constructor
     */
    template <typename ... Ts>
    inline void emplace_back(Ts && ... args)
    {
        unsafe_resize(size() + 1);
        new (end() - 1) T(std::forward<Ts>(args)...);
    }

    /**
     * @brief Removes all elements from the array
     */
    inline void clear()
    {
        resize(0);
    }

    /**
     * @brief Clears and deallocates the array
     */
    inline void reset()
    {
        clear();
        deallocate();
    }

    inline void shrink_to_fit()
    {
        if (size() == 0)
        {
            deallocate();
        }
        else
        {
            reallocate(size())
        }
    }

    /**
     * @brief Default constructor
     *
     * @param allocator Pointer to allocator instance
     */
    implicit Vector(A * allocator = A::instance()) : allocator_(allocator)
    {
    }

    /**
     * @brief Copy constructor
     *
     * @param other Instance to copy from
     */
    implicit Vector(Span<T> other, A * allocator = A::instance()) : Vector(allocator)
    {
        unsafe_resize(other.size());
        util::raw_copy_range(other.data(), data(), size());
    }

    /**
     * @brief Move constructor
     *
     * @param other Instance to move from
     */
    implicit Vector(Vector && other)
    {
        swap(*this, other);
    }

    /**
     * @brief Destructor
     */
    implicit ~Vector()
    {
        reset();
    }
};
}