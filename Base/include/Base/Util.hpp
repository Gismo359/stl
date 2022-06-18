/**
 * @file Util.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */


namespace std
{
template <typename T>
struct Span;
}

namespace util
{
/**
 * @brief Swaps two objects of the same type
 * 
 * @param a First object
 * @param b Second object
 */
template <meta::not_readonly T>
void swap(T & a, T & b)
{
    T t = std::reuse(a);
    a = std::reuse(b);
    b = std::reuse(t);
}

template <meta::not_readonly T>
void copy(T & a, T b)
{
    a = std::reuse(b);
}

template <meta::not_readonly T>
void raw_copy(T & a, T b)
{
    new (&a) T(std::reuse(b));
}

template <meta::not_readonly T>
void move(T & a, T & b)
{
    a = std::reuse(b);
}

template <meta::not_readonly T>
void raw_move(T & a, T & b)
{
    new (&a) T(std::reuse(b));
}

template <meta::not_readonly T, typename ... Ts>
void emplace(T & a, Ts ... args)
{
    a = T(std::forward<Ts>(args)...);
}

template <meta::not_readonly T, typename ... Ts>
void raw_emplace(T & a, Ts ... args)
{
    new (&a) T(std::forward<Ts>(args)...);
}

// TODO@Daniel:
//   These algorithms should check whether the two regions intersect

template <typename T>
void fill_range(T * a, Uint64 size, T const & value)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::copy(a[idx], value);
    }
}

template <typename T>
void fill_range(std::Span<T> a, T const & value)
{
    fill_range(a.data(), a.size(), value);
}

template <typename T>
void raw_fill_range(T * a, Uint64 size, T const & value)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::raw_copy(a[idx], value);
    }
}

template <typename T>
void raw_fill_range(std::Span<T> a, T const & value)
{
    raw_fill_range(a.data(), a.size(), value);
}

/**
 * @brief Copies an array of objects to another address
 * 
 * @param a Destination address
 * @param b Source address
 * @param size Length of both blocks
 */
template <typename T>
void copy_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::copy(a[idx], b[idx]);
    }
}

/**
 * @brief Copies objects from one span to another
 * 
 * @param a Destination span
 * @param b Source span
 */
template <typename T>
void copy_range(std::Span<T> a, std::Span<T> b)
{
    assert(a.size() == b.size());

    util::copy_range(a.data(), b.data(), a.data());
}

/**
 * @brief Copies an array of objects to uninitialized memory
 * 
 * @param a Destination address
 * @param b Source address
 * @param size Length of both blocks
 */
template <typename T>
void raw_copy_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::raw_copy(a[idx], b[idx]);
    }
}

/**
 * @brief Copies objects from one span to a span of unitialized memory
 * 
 * @param a Destination span
 * @param b Source span
 */
template <typename T>
void raw_copy_range(std::Span<T> a, std::Span<T> b)
{
    assert(a.size() == b.size());

    util::raw_copy_range(a.data(), b.data(), a.size());
}

/**
 * @brief Moves an array of objects to another address
 * 
 * @param a Destination address
 * @param b Destination address
 * @param size Length of both blocks
 */
template <meta::not_readonly T>
void move_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::move(a[idx], b[idx]);
    }
}

/**
 * @brief Moves objects from one span to another
 * 
 * @param a Destination span
 * @param b Source span
 */
template <typename T>
void move_range(std::Span<T> a, std::Span<T> b)
{
    assert(a.size() == b.size());

    util::move_range(a.data(), b.data(), a.size());
}

/**
 * @brief Moves an array of objects to uninitialized memory
 * 
 * @param a Destination address
 * @param b Source address
 * @param size Length of both blocks
 */
template <typename T>
void raw_move_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        util::raw_move(a[idx], b[idx]);
    }
}

/**
 * @brief Moves objects from one span to a span of unitialized memory
 * 
 * @param a Destination span
 * @param b Source span
 */
template <typename T>
void raw_move_range(std::Span<T> a, std::Span<T> b)
{
    assert(a.size() == b.size());

    util::raw_move_range(a.data(), b.data(), a.size());
}
}