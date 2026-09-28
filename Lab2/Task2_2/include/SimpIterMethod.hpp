#pragma once

#include "concepts.hpp"
#include "shared.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <expected>
#include <ranges>
#include <string_view>

namespace
{

template <concepts::SimpleIterSysEq S>
constexpr double calc_jacobian_norm(const typename S::XVec& x)
{
    const double row1_sum { std::abs(S::phi1dx1(x)) + std::abs(S::phi1dx2(x)) };
    const double row2_sum { std::abs(S::phi2dx1(x)) + std::abs(S::phi2dx2(x)) };

    return std::max(row1_sum, row2_sum);
}

constexpr double calc_mx_abs_diff(const auto& prev_x, const auto& x)
{
    double max_diff { 0.0 };
    for (auto&& [prev, curr] : std::views::zip(prev_x, x))
    {
        max_diff = std::max(max_diff, std::abs(curr - prev));
    }

    return max_diff;
}

} // namespace

template <concepts::SimpleIterSysEq S>
constexpr std::expected<Result, std::string_view> simple_iteration_method(double eps, size_t max_iter = 1000)
{
    typename S::XVec x { S::x0 };
    typename S::XVec prev_x {};

    const double q { calc_jacobian_norm<S>(x) };
    if (q >= 1.0) return std::unexpected("Convergence condition failed: ||phi'(x)|| >= 1");

    const double term_cond { (1.0 - q) / q * eps };

    size_t iterations { 0 }; 
    do
    {
        if (iterations >= max_iter) return std::unexpected("Simple iteration method reached maximum iterations.");

        prev_x = x;

        x[0] = S::phi1(prev_x);
        x[1] = S::phi2(prev_x);

        ++iterations;
    } while (calc_mx_abs_diff(prev_x, x) > term_cond);

    return Result{x, iterations};
}