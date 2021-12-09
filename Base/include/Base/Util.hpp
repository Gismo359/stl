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

namespace util
{
template <typename T>
struct Span;

/**
 * @brief Swaps two objects of the same type
 * 
 * @param a First object
 * @param b Second object
 */
template <typename T>
void swap(T & a, T & b)
{
    T t = std::move(a);
    a = std::move(b);
    b = std::move(t);
}

// TODO@Daniel:
//   These algorithms should check whether the two regions intersect

/**
 * @brief Moves an array of objects to another address
 * 
 * @param a Source address
 * @param b Destination address
 * @param size Length of both blocks
 */
template <typename T>
void move_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        b[idx] = std::move(a[idx]);
    }
}

/**
 * @brief Moves objects from one span to another
 * 
 * @param a Source span
 * @param b Destination span
 */
template <typename T>
void move_range(Span<T> a, Span<T> b)
{
    assert(a.size() == b.size());

    move_range(a.data(), b.data(), a.size());
}

/**
 * @brief Moves an array of objects to uninitialized memory
 * 
 * @param a Source address
 * @param b Destination address
 * @param size Length of both blocks
 */
template <typename T>
void raw_move_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        new (&b[idx]) T(std::move(a[idx]));
    }
}

/**
 * @brief Moves objects from one span to a span of unitialized memory
 * 
 * @param a Source span
 * @param b Destination span
 */
template <typename T>
void raw_move_range(Span<T> a, Span<T> b)
{
    assert(a.size() == b.size());

    raw_move_range(a.data(), b.data(), a.size());
}

/**
 * @brief Copies an array of objects to another address
 * 
 * @param a Source address
 * @param b Destination address
 * @param size Length of both blocks
 */
template <typename T>
void copy_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        b[idx] = a[idx];
    }
}

/**
 * @brief Copies objects from one span to another
 * 
 * @param a Source span
 * @param b Destination span
 */
template <typename T>
void copy_range(Span<T> a, Span<T> b)
{
    assert(a.size() == b.size());

    copy_range(a.data(), b.data(), a.data());
}

/**
 * @brief Copies an array of objects to uninitialized memory
 * 
 * @param a Source address
 * @param b Destination address
 * @param size Length of both blocks
 */
template <typename T>
void raw_copy_range(T * a, T * b, Uint64 size)
{
    for (Uint64 idx = 0; idx < size; ++idx)
    {
        new (&b[idx]) T(a[idx]);
    }
}

/**
 * @brief Copies objects from one span to a span of unitialized memory
 * 
 * @param a Source span
 * @param b Destination span
 */
template <typename T>
void raw_copy_range(Span<T> a, Span<T> b)
{
    assert(a.size() == b.size());

    raw_copy_range(a.data(), b.data(), a.size());
}
}