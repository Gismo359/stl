/**
 * @file Math.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

namespace math
{
template <typename T>
constexpr macro Int8 sign(T value)
{
    return (T { 0 } < value) - (value < T { 0 });
}

template <typename T>
constexpr macro T abs(T value)
{
    return value < 0 ? -value : value;
}
}