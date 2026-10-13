#pragma once

#include "common.hpp"

#include <expected>
#include <span>
#include <algorithm>
#include <string_view>

namespace math
{

enum class differentiation_error
{
    insufficient_points,
    out_of_bounds,
    invalid_arguments,
    division_by_zero
};

constexpr std::string_view error_to_string(
    math::differentiation_error err
) noexcept
{
    switch (err)
    {
        case math::differentiation_error::insufficient_points:
            return "Error: Insufficient points for second-order approximation.";
        case math::differentiation_error::out_of_bounds:
            return "Error: Target X is out of interpolation bounds.";
        case math::differentiation_error::invalid_arguments:
            return "Error: X and Y array sizes do not match.";
        case math::differentiation_error::division_by_zero:
            return "Error: Division by zero detected (duplicate X nodes).";
        default:
            return "Error: Unknown math error.";
    }
}

struct derivative_result
{
    double first_derivative;
    double second_derivative;
    size_t used_start_index;
};

[[nodiscard]]
constexpr auto calculate_divided_diff_1(
    std::span<const double> x,
    std::span<const double> y,
    std::size_t i
) noexcept
    -> std::expected<double, differentiation_error>
{
    const double dx { x[i + 1] - x[i] };
    if (std::abs(dx) < constants::eps) [[unlikely]]
    {
        return std::unexpected(differentiation_error::division_by_zero);
    }
    
    return (y[i + 1] - y[i]) / dx;
}

[[nodiscard]]
constexpr auto calculate_divided_diff_2(
    std::span<const double> x,
    std::span<const double> y,
    std::size_t i
) noexcept
    -> std::expected<double, differentiation_error>
{
    const auto f1_right { calculate_divided_diff_1(x, y, i + 1) };
    const auto f1_left  { calculate_divided_diff_1(x, y, i) };

    if (!f1_right.has_value() || !f1_left.has_value()) [[unlikely]]
    {
        return std::unexpected(differentiation_error::division_by_zero);
    }

    const double dx { x[i + 2] - x[i] };
    if (std::abs(dx) < constants::eps) [[unlikely]]
    {
        return std::unexpected(differentiation_error::division_by_zero);
    }

    return (f1_right.value() - f1_left.value()) / dx;
}

[[nodiscard]]
constexpr size_t find_optimal_index(
    std::span<const double> x,
    double target_x
) noexcept
{
    const size_t size { x.size() };
    auto it { std::ranges::lower_bound(x, target_x) };
    size_t idx { static_cast<size_t>(std::distance(x.begin(), it)) };

    if (idx == 0)
    {
        return 0;
    }
    else if (idx >= size - 1)
    {
        return size - 3;
    }
    
    return (idx - 1);
}

[[nodiscard]]
constexpr auto calculate_derivatives(
    std::span<const double> x,
    std::span<const double> y,
    double target_x
) noexcept
    -> std::expected<derivative_result, differentiation_error>
{
    if (x.size() != y.size()) [[unlikely]]
    {
        return std::unexpected(differentiation_error::invalid_arguments);
    }
    
    if (x.size() < 3) [[unlikely]]
    {
        return std::unexpected(differentiation_error::insufficient_points);
    }
    
    if (target_x < x.front() || target_x > x.back()) [[unlikely]]
    {
        return std::unexpected(differentiation_error::out_of_bounds);
    }

    const size_t i { find_optimal_index(x, target_x) };

    const auto f1 { calculate_divided_diff_1(x, y, i) };
    const auto f2 { calculate_divided_diff_2(x, y, i) };

    if (!f1.has_value() || !f2.has_value()) [[unlikely]]
    {
        return std::unexpected(differentiation_error::division_by_zero);
    }

    const double first_deriv { f1.value() + f2.value() * (2.0 * target_x - x[i] - x[i + 1]) };
    const double second_deriv { 2.0 * f2.value() };

    return derivative_result
    {
        .first_derivative = first_deriv,
        .second_derivative = second_deriv,
        .used_start_index = i
    };
}

} // namespace math