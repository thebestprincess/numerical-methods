#pragma once

#include <concepts>

namespace concepts
{

template <typename T>
concept FuncDeriv = requires (
    const T& func,
    double x)
{
    { T::x0 } -> std::convertible_to<double>;

    { func.calc_fst_dv(x) } -> std::same_as<double>;
    { func.calc_snd_dv(x) } -> std::same_as<double>;
    { func.calc_func(x) } -> std::same_as<double>;
    { func.calc_simp_iter(x)} -> std::same_as<double>;
    { func.calc_simp_iter_dv(x)} -> std::same_as<double>;
    { func.get_a() } -> std::convertible_to<double>;
    { func.get_b() } -> std::convertible_to<double>;
};

} // namespace concepts