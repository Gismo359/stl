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
    Uint64 size_ = 0;

protected:
    /**
     * @brief Internal setter for Span::data()
     * 
     * @param data New pointer to first element
     */
    void set_data(Pointer data)
    {
        data_ = data;
    }

    /**
     * @brief Internal setter for Span::size()
     * 
     * @param data New size
     */
    void set_size(Uint64 size)
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
    Span(Pointer data, Uint64 size) : data_(data), size_(size)
    {
    }

    /**
     * @brief Construct a new Span object
     * 
     * @param begin Pointer to the first element in the span
     * @param end Pointer to one past the last element in the span
     */
    Span(Pointer begin, Pointer end) : Span(begin, end - begin)
    {
    }

    /**
     * @return Number of elements in this span
     */
    Uint64 size() const
    {
        return size_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    ConstPointer data() const
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    Pointer data()
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    ConstPointer begin() const
    {
        return data_;
    }

    /**
     * @return Pointer to the first element in the span
     */
    Pointer begin()
    {
        return data_;
    }

    /**
     * @return Pointer to one past the last element in the span
     */
    ConstPointer end() const
    {
        return data_ + size_;
    }

    /**
     * @return Pointer to one past the last element in the span
     */
    Pointer end()
    {
        return data_ + size_;
    }

    /**
     * @param idx 
     * @return Item at index idx
     */
    ConstReference at(Int64 idx) const
    {
        assert(idx >= 0);
        
        return data_[idx];
    }

    /**
     * @param idx 
     * @return Item at index idx
     */
    Reference at(Int64 idx)
    {
        assert(idx >= 0);
        
        return data_[idx];
    }

    /**
     * @param idx 
     * @return Item at index idx
     */
    ConstReference operator[](Int64 idx) const
    {
        assert(idx >= 0);
        
        return at(idx);
    }

    /**
     * @param idx 
     * @return Item at index idx
     */
    Reference operator[](Int64 idx)
    {
        assert(idx >= 0);
        
        return at(idx);
    }

    /**
     * @param start
     * @param stop
     * @return Sub-span from start to stop
     */
    Span<T const> middle(Int64 start, Int64 stop) const
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
    Span<T> middle(Int64 start, Int64 stop)
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
    Span<T const> left(Int64 stop) const
    {
        assert(stop >= 0);
        
        return middle(0, stop);
    }

    /**
     * @param stop
     * @return Sub-span from beginning to stop
     */
    Span<T> left(Int64 stop)
    {
        assert(stop >= 0);
        
        return middle(0, stop);
    }

    /**
     * @param start
     * @return Sub-span from start to the end
     */
    Span<T const> right(Int64 start) const
    {
        assert(start >= 0);
        
        return middle(start, size_);
    }

    /**
     * @param start
     * @return Sub-span from start to the end
     */
    Span<T> right(Int64 start)
    {
        assert(start >= 0);
        
        return middle(start, size_);
    }

    Span<T> const & me() const
    {
        return *this;
    }

    Span<T> & me()
    {
        return *this;
    }

    template <typename F>
    Span<T> const & operator<<(iterate::visitor<F, iterate::forward> f) const
    {
        T * b = data();
        T * e = data() + size();
        while (b != e) f(*b++);
        return *this;
    }

    template <typename F>
    Span<T> & operator<<(iterate::visitor<F, iterate::forward> f)
    {
        T * b = data();
        T * e = data() + size();
        while (b != e) f(*b++);
        return *this;
    }

    template <typename F>
    Span<T> const & operator<<(iterate::visitor<F, iterate::backward> f) const
    {
        T * b = data();
        T * e = data() + size();
        while (b != e) f(*--e);
        return *this;
    }

    template <typename F>
    Span<T> & operator<<(iterate::visitor<F, iterate::backward> f)
    {
        T * b = data();
        T * e = data() + size();
        while (b != e) f(*--e);
        return *this;
    }

    /**
     * @brief Default copy assignment operator
     */
    Span<T> & operator=(Span const & other) = default;
    
    /**
     * @brief Default move assignment operator
     */
    Span<T> & operator=(Span && other) = default;

    /**
     * @brief Default constructor
     */
    implicit Span() = default;

    /**
     * @brief Default copy constructor
     */
    implicit Span(Span const & other) = default;
    
    /**
     * @brief Default move constructor
     */
    implicit Span(Span && other) = default;

    /**
     * @brief Default destructor
     */
    implicit ~Span() = default;
};
}