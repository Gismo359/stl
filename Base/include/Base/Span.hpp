/**
 * @file Span.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief A view of an array of objects
 * @version 0.1
 * @date 2021-12-07
 *
 * @copyright Copyright (c) 2021
 *
 */

namespace std
{
/**
 * @brief A view of an array of objects
 *
 * @tparam T Type of underlying objects
 */
template <typename T>
struct Span
{
private:
    using Base = mem::Block;
    using This = Span<T>;

public:
    using Pointer = T *;
    using ConstPointer = T const *;
    using Reference = T &;
    using ConstReference = T const &;

private:
    Pointer data_ = null;
    u64 size_ = 0;

protected:
    /**
     * @brief Internal setter for Span::data()
     *
     * @param data New pointer to first element
     */
    macro void set_data(Pointer data)
    {
        data_ = data;
    }

    /**
     * @brief Internal setter for Span::size()
     *
     * @param data New size
     */
    macro void set_size(u64 size)
    {
        size_ = size;
    }

public:
    /**
     * @brief Construct a new Span object
     *
     * @param data Pointer to the first element in the span
     * @param size Number of elements in this span
     */
    implicit macro Span(Pointer data, u64 size) : data_(data), size_(size)
    {
    }

    /**
     * @brief Construct a new Span object
     *
     * @param begin Pointer to the first element in the span
     * @param end Pointer to one past the last element in the span
     */
    implicit macro Span(Pointer begin, Pointer end) : Span(begin, end - begin)
    {
    }

    /**
     * @return Number of elements in this span
     */
    macro u64 size() const
    {
        return size_;
    }

    /**
     * @return Whether this span is empty or not
     */
    macro bool empty() const
    {
        return size() == 0;
    }

    /**
     * @return Pointer to the first element in the span
     */
    macro ConstPointer data() const
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    macro Pointer data()
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    macro ConstPointer begin() const
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    macro Pointer begin()
    {
        return data_;
    }

    /**
     * @return Pointer to one past the last element in the span
     */
    macro ConstPointer end() const
    {
        return data_ + size_;
    }

    /**
     * @return Pointer to one past the last element in the span
     */
    macro Pointer end()
    {
        return data_ + size_;
    }

    /**
     * @param idx
     * @return Item at index idx
     */
    macro ConstReference at(i64 idx) const
    {
        assert(idx >= 0);
        assert(idx < size());

        return data_[idx];
    }

    /**
     * @param idx
     * @return Item at index idx
     */
    macro Reference at(i64 idx)
    {
        assert(idx >= 0);
        assert(idx < size());

        return data_[idx];
    }

    /**
     * @param idx
     * @return Item at index idx
     */
    macro ConstReference operator[](i64 idx) const
    {
        assert(idx >= 0);
        assert(idx < size());

        return at(idx);
    }

    /**
     * @param idx
     * @return Item at index idx
     */
    macro Reference operator[](i64 idx)
    {
        assert(idx >= 0);
        assert(idx < size());

        return at(idx);
    }

    macro Span<T const> copy() const
    {
        return *this;
    }

    macro Span<T> copy()
    {
        return *this;
    }

    /**
     * @param start
     * @param stop
     * @return Sub-span from start to stop
     */
    macro Span<T const> middle(i64 start, i64 stop) const
    {
        assert(start <= stop);
        assert(start >= 0);
        assert(stop >= 0);

        return Span(data_ + start, data_ + stop);
    }

    /**
     * @param start
     * @param stop
     * @return Sub-span from start to stop
     */
    macro Span<T> middle(i64 start, i64 stop)
    {
        assert(start <= stop);
        assert(start >= 0);
        assert(stop >= 0);

        return Span(data_ + start, data_ + stop);
    }

    /**
     * @param stop
     * @return Sub-span from beginning to stop
     */
    macro Span<T const> left(i64 stop) const
    {
        assert(stop >= 0);

        return middle(0, stop);
    }

    /**
     * @param stop
     * @return Sub-span from beginning to stop
     */
    macro Span<T> left(i64 stop)
    {
        assert(stop >= 0);

        return middle(0, stop);
    }

    /**
     * @param start
     * @return Sub-span from start to the end
     */
    macro Span<T const> right(i64 start) const
    {
        assert(start >= 0);

        return middle(start, size_);
    }

    /**
     * @param start
     * @return Sub-span from start to the end
     */
    macro Span<T> right(i64 start)
    {
        assert(start >= 0);

        return middle(start, size_);
    }

    macro Span<T> const & me() const
    {
        return *this;
    }

    macro Span<T> & me()
    {
        return *this;
    }

    implicit macro operator Span<T const>() const
    {
        return copy();
    }

    template <typename F>
    macro void operator<<(iterate::visitor<F, iterate::forward> f) const
    {
        T * b = data();
        T * e = data() + size();
        while (b != e)
        {
            f(*b++);
        }
    }

    template <typename F>
    macro void operator<<(iterate::visitor<F, iterate::forward> f)
    {
        T * b = data();
        T * e = data() + size();
        while (b != e)
        {
            f(*b++);
        }
    }

    template <typename F>
    macro void operator<<(iterate::visitor<F, iterate::backward> f) const
    {
        T * b = data();
        T * e = data() + size();
        while (b != e)
        {
            f(*--e);
        }
    }

    template <typename F>
    macro void operator<<(iterate::visitor<F, iterate::backward> f)
    {
        T * b = data();
        T * e = data() + size();
        while (b != e)
        {
            f(*--e);
        }
    }

    /**
     * @brief Default copy assignment operator
     */
    macro Span<T> & operator=(Span const & other) = default;

    /**
     * @brief Default move assignment operator
     */
    macro Span<T> & operator=(Span && other) = default;

    /**
     * @brief Default constructor
     */
    implicit macro Span() = default;

    /**
     * @brief Default copy constructor
     */
    implicit macro Span(Span const & other) = default;

    /**
     * @brief Default move constructor
     */
    implicit macro Span(Span && other) = default;

    /**
     * @brief Default destructor
     */
    implicit macro ~Span() = default;
};
}