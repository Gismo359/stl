/**
 * @file Traits.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */
namespace std
{
template <typename T> struct noref       { using type = T; };
template <typename T> struct noref<T &>  { using type = T; };
template <typename T> struct noref<T &&> { using type = T; };
template <typename T> using noref_t = typename noref<T>::type;

template <typename T, typename U>
struct is_same
{
    constant auto value = false;
};

template <typename T>
struct is_same<T, T>
{
    constant auto value = true;
};

template <typename T, typename U>
constant auto is_same_v = is_same<T, U>::value;

template <typename T>
constant auto is_const = false;

template <typename T>
constant auto is_const<T const> = true;

template< class T >
std::noref_t<T> && reuse(T && t)
{
    return cast(std::noref_t<T> &&, t);
}

template <typename T>
T && forward(std::noref_t<T> & t)
{
    return cast(T &&, t);
}

template <typename T>
T && forward(std::noref_t<T> && t)
{
    return cast(T &&, t);
}
}