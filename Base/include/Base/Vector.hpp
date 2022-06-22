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
 * @tparam Z Whether this vector type is null terminated
 */
template <typename T, typename A = mem::SystemAllocator, bool Z = false>
struct Vector : Span<T>
{
private:
    using Base = Span<T>;
    using This = Vector<T, A>;

protected:
    u64 allocated_size_ = 0;
    A * allocator_ = null;

    /**
     * @return A `mem::Block` equivalent to the internal block managed by this vector
     */
    macro mem::Block memory_block()
    {
        return mem::Block { data(), allocated_size_ };
    }

    /**
     * @brief Internal setter for `Vector::capacity()`
     *
     * @param new_allocated_size New size of allocated memory
     */
    macro void set_allocated_size(u64 new_allocated_size)
    {
        allocated_size_ = new_allocated_size;
    }

    /**
     * @brief Reserves and sets `Vector::size()`, but does not (de)initialize invalidated
     * objects
     *
     * @param size New `Vector::size()` of this vector
     */
    macro void unsafe_resize(u64 size)
    {
        reserve(size + Z);
        set_size(size);
    }

    /**
     * @brief Attempt to reallocate the
     *
     * @param new_capacity
     * @return Whether the reallocation succeeded
     */
    macro bool reallocate(u64 new_capacity)
    {
        mem::Block block = memory_block();
        if (allocator().reallocate(block, (new_capacity + Z) * sizeof(T)))
        {
            set_allocated_size(block.size);
            return true;
        }
        return false;
    }

    /**
     * @brief Deallocates the internal memory block
     */
    macro void deallocate()
    {
        mem::Block block = memory_block();
        allocator().deallocate(block);
    }

    /**
     * @brief Deallocates the internal memory block and tears down the vector
     */
    macro void tear_down()
    {
        deallocate();

        set_data(null);
        set_size(0);
        set_allocated_size(0);
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
    macro u64 capacity() const
    {
        return allocated_size_ / sizeof(T) - Z * sizeof(T);
    }

    /**
     * @brief Make sure the vector has enough space for `new_capacity` elements without
     * needing to (re)allocate the internal memory block
     *
     * @param new_capacity Number of elements that this vector should be able to hold
     */
    macro void reserve(u64 new_capacity)
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

        Base span { cast(Pointer, block.data), size() };
        util::raw_move_range(span, *this);
        deallocate();

        set_data(span.data());
        set_allocated_size(block.size);
    }

    /**
     * @brief Sets the `Vector::size()` of this vector and (de)initializes items as needed
     *
     * If `new_size` is larger than `Vector::size()`, then this calls `Vector::reserve`
     * and may trigger a (re)allocation
     *
     * @param new_size New number of elements in this vector
     * @param value Initial value for new elements when `new_size > Vector::size`
     */
    macro void resize(u64 new_size, T const & value = T {})
    {
        u64 old_size = size();
        if (new_size <= old_size)
        {
            right(new_size).iter(T & item)
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
        u64 old_size = size();
        unsafe_resize(old_size + 1);
        util::raw_move(at(old_size), item);
    }

    /**
     * @brief Creates an object at the end of the array
     *
     * @param args Arguments, forwarded to the new object's constructor
     */
    template <typename... Ts>
    macro void emplace_back(Ts &&... args)
    {
        u64 old_size = size();
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
        tear_down();
    }

    /**
     * @brief Minimize the difference between `Vector::capacity()` and `Vector::size()`
     *
     * NOTE: 
     * 
     * @param force Whether to try and force an allocation if reallocation fails
     */
    macro void shrink_to_fit(bool force = false)
    {
        if (size() == 0)
        {
            tear_down();
        }
        else if (not reallocate(size()) and force)
        {
            // TODO@Daniel: Implement
        }
    }

    /**
     * @brief Copy assignment operator
     */
    macro Vector<T> & operator=(Vector const & other)
    {
        clear();
        unsafe_resize(other.size());
        util::raw_copy_range(*this, other);
    }

    /**
     * @brief Move assignment operator
     */
    macro Vector<T> & operator=(Vector && other)
    {
        deallocate();
        util::swap(*this, other);
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
    implicit macro Vector(Span<T> other, A * allocator = A::instance()) :
        Vector(allocator)
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
        util::swap(*this, other);
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