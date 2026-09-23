#pragma once

#include <cmath>

class Func final
{
    static constexpr double a { 0.0 };
    static constexpr double b { 0.2 };
public:
    static constexpr double x0 { b };

    static constexpr double get_a() noexcept { return a; }
    static constexpr double get_b() noexcept { return b; }

    static double calc_func(double x) noexcept { return std::sqrt(1.0 - x * x) - std::exp(x) + 0.1; }
    static double calc_fst_dv(double x) noexcept { return -x / std::sqrt(1.0 - x * x) - std::exp(x); }
    static double calc_snd_dv(double x) noexcept { return -1.0 / std::pow(1.0 - x * x, 1.5) - std::exp(x); }    
    static double calc_simp_iter(double x) noexcept { return std::log(std::sqrt(1 - x * x) + 0.1); }
    static double calc_simp_iter_dv(double x) noexcept { return -x / (1 - x * x + 0.1 * std::sqrt(1 - x * x)); }
};
