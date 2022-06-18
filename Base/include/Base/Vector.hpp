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
     * @param new_capacity New capacity
     */
    macro void set_capacity(Uint64 new_capacity)
    {
        capacity_ = new_capacity;
    }

    /**
     * @brief Reserves and sets Vector::size(), but does not (de)initialize invalidated objects
     *
     * @param size
     */
    macro void unsafe_resize(Uint64 size)
    {
        reserve(size);
        set_size(size);
    }

    /**
     * @brief Attempt to reallocate the
     *
     * @param new_capacity
     * @return Bool
     */
    macro Bool reallocate(Uint64 new_capacity)
    {
        mem::Block block{ data(), capacity_ * sizeof(T) };
        if (allocator().reallocate(block, new_capacity * sizeof(T)))
        {
            set_capacity(new_capacity);
            return true;
        }
        return false;
    }

    macro void deallocate()
    {
        mem::Block block{ data(), capacity_ * sizeof(T) };
        allocator().deallocate(block);
    }

public:
    /**
     * @return This instance's allocator
     */
    macro A const & allocator() const
    {
        return *allocator_;
    }

    /**
     * @return This instance's allocator
     */
    macro A & allocator()
    {
        return *allocator_;
    }

    /**
     * @return Number of elements this vector can hold before reallocating
     */
    macro Uint64 capacity() const
    {
        return capacity_;
    }

    macro void reserve(Uint64 new_capacity)
    {
        if (new_capacity <= capacity())
        {
            return;
        }

        if (reallocate(new_capacity))
        {
            return;
        }

        mem::Block block = allocator().allocate(new_capacity * sizeof(T));
        assert(block);

        Base span{ cast(Pointer, block.data), size() };
        util::raw_move_range(span, *this);
        deallocate();

        set_data(span.data());
        set_capacity(new_capacity);
    }

    macro void resize(Uint64 new_size, T const & value = T{})
    {
        Uint64 old_size = size();
        if (new_size <= old_size)
        {
            right(new_size).iter(auto & item)
            {
                item.~T();
            };
            set_size(new_size);
        }
        else
        {
            unsafe_resize(new_size);
            util::raw_fill_range(right(old_size), value);
        }
    }

    /**
     * @brief Inserts an object at the end of the array
     *
     * @param item Object to insert
     */
    macro void push_back(T item)
    {
        Uint64 old_size = size();
        unsafe_resize(old_size + 1);
        util::raw_move(at(old_size), item);
    }

    /**
     * @brief Creates an object at the end of the array
     *
     * @param args Arguments, forwarded to the new object's constructor
     */
    template <typename ... Ts>
    macro void emplace_back(Ts && ... args)
    {
        Uint64 old_size = size();
        unsafe_resize(old_size + 1);
        util::raw_emplace(at(old_size), std::forward<Ts>(args)...);
    }

    /**
     * @brief Removes all elements from the array
     */
    macro void clear()
    {
        resize(0);
    }

    /**
     * @brief Clears and deallocates the array
     */
    macro void reset()
    {
        clear();
        deallocate();
    }

    macro void shrink_to_fit()
    {
        if (size() == 0)
        {
            deallocate();
        }
        else
        {
            reallocate(size());
        }
    }

    /**
     * @brief Default constructor
     *
     * @param allocator Pointer to allocator instance
     */
    implicit macro Vector(A * allocator = A::instance()) : allocator_(allocator)
    {
    }

    /**
     * @brief Copy constructor
     *
     * @param other Instance to copy from
     */
    implicit macro Vector(Span<T> other, A * allocator = A::instance()) : Vector(allocator)
    {
        unsafe_resize(other.size());
        util::raw_copy_range(*this, other);
    }

    /**
     * @brief Move constructor
     *
     * @param other Instance to move from
     */
    implicit macro Vector(Vector && other)
    {
        swap(*this, other);
    }

    /**
     * @brief Destructor
     */
    implicit macro ~Vector()
    {
        reset();
    }
};
}