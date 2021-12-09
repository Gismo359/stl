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

template< class T >
std::noref_t<T> && move(T && t)
{
    return cast(std::noref_t<T> &&, t);
}

template< class T >
T && forward(std::noref_t<T> & t)
{
    return t;
}

template< class T >
T && forward(std::noref_t<T> && t)
{
    return t;
}
}