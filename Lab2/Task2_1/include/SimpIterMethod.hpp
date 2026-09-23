#pragma once

#include "concepts.hpp"
#include "shared.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <expected>
#include <string_view>
#include <utility>

template <concepts::FuncDeriv Func>
std::pair<bool, double> check_simp_iter_method_convergence(double eps)
{
    double left_endpoint { Func::calc_simp_iter_dv(Func::get_a()) };
    double right_endpoint { Func::calc_simp_iter_dv(Func::get_b()) };

    double mx_abs_endpoint { (std::max(std::abs(left_endpoint), std::abs(right_endpoint))) };

    bool is_convergent { mx_abs_endpoint - 1.0 < eps ? true : false };
    return {is_convergent, mx_abs_endpoint};
} 

constexpr bool check_simp_iter_condition(double prev_x, double x, double q, double eps)
{
    return q / (1 - q) * std::abs(x - prev_x) <= eps;
}

template <concepts::FuncDeriv Func>
std::expected<Result, std::string_view> simp_iter_method(double eps)
{
    auto result { check_simp_iter_method_convergence<Func>(eps) };
    if (!result.first) 
        return std::unexpected("The condition of the theorem is not satisfied: |phi(x)'| >= 1.0, for all x in [a, b]");

    double prev_x { 0.0 };
    double x{ (Func::get_a() + Func::get_b()) / 2};
    double q { result.second };
    size_t iterations { 0 };
    do
    {
        prev_x = x;
        x = Func::calc_simp_iter(prev_x); 

        ++iterations;
    } while(!check_simp_iter_condition(prev_x, x, q, eps));

    return Result{ x, iterations };
}