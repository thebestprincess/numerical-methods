#pragma once

#include <cmath>
#include <concepts>

namespace integration
{

[[nodiscard]]
constexpr double left_rectangles(
    std::invocable<double> auto&& f,
    double a,
    double h,
    int n
) noexcept
{
    double sum { 0.0 };
    for (int i { 0 }; i < n; ++i)
    {
        sum += f(a + static_cast<double>(i) * h);
    }
    return sum * h;
}

[[nodiscard]]
constexpr double right_rectangles(
    std::invocable<double> auto&& f,
    double a,
    double h,
    int n
) noexcept
{
    double sum { 0.0 };
    for (int i { 1 }; i <= n; ++i)
    {
        sum += f(a + static_cast<double>(i) * h);
    }
    return sum * h;
}

[[nodiscard]] constexpr double middle_rectangles(
    std::invocable<double> auto&& f,
    double a,
    double h,
    int n
) noexcept
{
    double sum { 0.0 };
    for (int i { 0 }; i < n; ++i)
    {
        sum += f(a + static_cast<double>(i) * h + (h / 2.0));
    }
    return sum * h;
}

[[nodiscard]]
constexpr double trapezoids(
    std::invocable<double> auto&& f,
    double a,
    double b,
    double h,
    int n
) noexcept
{
    double sum { (f(a) + f(b)) / 2.0 };
    for (int i { 1 }; i < n; ++i)
    {
        sum += f(a + static_cast<double>(i) * h);
    }
    return sum * h;
}

[[nodiscard]]
constexpr double simpson(
    std::invocable<double> auto&& f,
    double a,
    double b,
    double h,
    int n
) noexcept
{
    double sum { f(a) + f(b) };
    for (int i { 1 }; i < n; ++i)
    {
        double const weight { (i % 2 == 0) ? 2.0 : 4.0 };
        sum += weight * f(a + static_cast<double>(i) * h);
    }
    return sum * (h / 3.0);
}

struct runge_romberg_result
{
    double refined_value;
    double error_estimate;
};

[[nodiscard]]
constexpr runge_romberg_result apply_runge_romberg(
    double i_h1,
    double i_h2,
    int p
) noexcept
{
    double const factor { static_cast<double>((1 << p) - 1) };
    double const error { (i_h2 - i_h1) / factor };
    
    return { i_h2 + error, std::abs(error) };
}

} // namespace integration