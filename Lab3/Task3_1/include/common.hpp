#pragma once

#include <array>
#include <cmath>
#include <numbers>

inline double function(double x) noexcept
{
    return std::tan(x);
}

namespace constants
{

inline constexpr std::array<double, 4> set_of_points1
{
    0.0,
    std::numbers::pi / 8.0,
    2.0 * std::numbers::pi / 8.0,
    3.0 * std::numbers::pi / 8.0
};
inline constexpr std::array<double, 4> set_of_points2
{
    0.0,
    std::numbers::pi / 8.0,
    std::numbers::pi / 3.0,
    3.0 * std::numbers::pi / 8.0
};
inline constexpr double x_star { 3.0 * std::numbers::pi / 16.0 };

} // namespace constants