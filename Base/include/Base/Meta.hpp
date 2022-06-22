/**
 * @file Meta.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

namespace meta
{
template <typename T>
concept readonly = std::is_const<std::noref_t<T>>;

template <typename T>
concept not_readonly = not std::is_const<std::noref_t<T>>;
}