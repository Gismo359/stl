namespace iterate
{
template <typename F, typename G>
struct visitor
{
private:
    F f_;

public:
    macro implicit visitor(F f) : f_(std::move(f))
    {
    }

    template <typename ... Ts>
    macro auto operator()(Ts && ... args) const
    {
        return f_(std::forward<Ts>(args)...);
    }

    template <typename ... Ts>
    macro auto operator()(Ts && ... args)
    {
        return f_(std::forward<Ts>(args)...);
    }
};

template <typename T, typename U>
struct iterator
{
private:
    T * object_ = null;

public:
    macro T * object()
    {
        return object_;
    }

    implicit macro iterator(T * object = null) : object_(object)
    {
    }

    implicit macro iterator() = default;
    implicit macro ~iterator() = default;
};

template <typename T>
struct trait
{
};

template <typename T>
struct is_visitor
{
    constant auto value = false;
};

template <typename T, typename U>
struct is_visitor<visitor<T, U>>
{
    constant auto value = true;
};

template <typename T>
struct is_iterator
{
    constant auto value = false;
};

template <typename T, typename U>
struct is_iterator<iterator<T, U>>
{
    constant auto value = true;
};

template <typename T>
struct is_trait
{
    constant auto value = false;
};

template <typename T>
struct is_trait<trait<T>>
{
    constant auto value = true;
};

template <typename T>
constant auto is_visitor_v = is_visitor<std::noref_t<T>>::value;
template <typename T>
constant auto is_iterator_v = is_iterator<std::noref_t<T>>::value;
template <typename T>
constant auto is_trait_v = is_trait<std::noref_t<T>>::value;

struct none
{
};

struct agane
{
};

struct forward
{
};

struct backward
{
};

internal macro auto me()
{
    return trait<none>{};
}

template <typename T, typename U, typename F>
requires (!is_visitor_v<F> && !is_iterator_v<F> && !is_trait_v<F>)
internal macro auto operator<<(iterator<T, trait<U>> it, F f)
{
    return it.object()->operator<<(visitor<F, U>(std::move(f)));
}

template <typename T, typename U>
requires (!is_visitor_v<T> && !is_iterator_v<T> && !is_trait_v<T>)
internal macro auto operator<<(T && container, trait<U>)
{
    return iterator<std::noref_t<T>, trait<U>> { &container };
}

template <typename T>
requires (!is_visitor_v<T> && !is_iterator_v<T> && !is_trait_v<T>)
internal macro auto operator<<(T && container, trait<none>)
{
    return iterator<std::noref_t<T>, trait<forward>> { &container };
}

template <typename T, typename U, typename V>
internal macro auto operator<<(iterator<T, trait<U>> it, trait<V>)
{
    if constexpr (std::is_same_v<V, none>)
    {
        return iterator<T, trait<U>> { it.object() };
    }
    else
    {
        return iterator<T, trait<V>> { it.object() };
    }
}
}