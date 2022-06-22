/**
 * @file Range.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

namespace std
{
template <typename T = i64>
struct Range
{
private:
    T start = 0;
    T stop = 0;
    T step = 0;

public:
    template <typename F>
    constexpr macro void operator<<(iterate::visitor<F, iterate::forward> f) const
    {
        T last = stop - step - (stop - start) % step;
        for (T idx = start; idx != last; idx += step)
        {
            f(idx);
        }
    }

    template <typename F>
    constexpr macro void operator<<(iterate::visitor<F, iterate::backward> f) const
    {
        T last = stop - step - (stop - start) % step;
        for (T idx = last; idx != start; idx -= step)
        {
            f(idx);
        }
    }

    /**
     * @brief Construct a range from `start` to `stop` by `step`
     *
     * @param start Beginning of this range, inclusive
     * @param stop End of this range, exclusive
     * @param step Increment of this range
     */
    implicit constexpr macro Range(T start, T stop, T step) :
        start(start), stop(stop), step(step)
    {
        assert(step != 0);
        assert(stop == start || math::sign(stop - start) == math::sign(step));
    }

    /**
     * @brief Construct a range from `start` to `stop` by `1`
     *
     * @param start Beginning of this range, inclusive
     * @param stop End of this range, exclusive
     */
    implicit constexpr macro Range(T start, T stop) :
        Range(start, stop, math::sign(stop - start))
    {
    }

    /**
     * @brief Construct a range from `0` to `stop` by `1`
     *
     * @param stop End of this range, exclusive
     */
    implicit constexpr macro Range(T stop) : Range(0, stop)
    {
    }

    /**
     * @brief Default constructor
     */
    implicit constexpr macro Range() = default;

    /**
     * @brief Default copy constructor
     */
    implicit constexpr macro Range(Range const & other) = default;

    /**
     * @brief Default move constructor
     */
    implicit constexpr macro Range(Range && other) = default;

    /**
     * @brief Default destructor
     */
    implicit constexpr macro ~Range() = default;
};
}