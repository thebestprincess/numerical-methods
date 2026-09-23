#pragma once

#include "concepts.hpp"
#include "shared.hpp"

#include <cmath>
#include <cstddef>
#include <expected>
#include <string_view>

template<concepts::FuncDeriv T>
constexpr bool check_newton_method_convergence(double x)
{
    if (T::calc_func(x) * T::calc_snd_dv(x) <= 0.0) return false;
    return true;
}

constexpr bool check_newton_term_condition(double x, double prev_x, double eps)
{
    return std::abs(x - prev_x) < eps;
}

template<concepts::FuncDeriv T>
constexpr std::expected<Result, std::string_view> newton_method(double eps)
{
    if (!check_newton_method_convergence<T>(T::x0))
        return std::unexpected("The product of the function and its second derivative is less than or equals to zero");

    double x { T::x0 };
    double prev_x { 0.0 };
    size_t iterations { 0uz };
    do
    {
        prev_x = x;
        
        double fx { T::calc_func(prev_x) };
        double fdx { T::calc_fst_dv(prev_x) };

        x = prev_x - fx / fdx;

        ++iterations;
    } while(!check_newton_term_condition(x, prev_x, eps));

    return Result{x, iterations};
}
