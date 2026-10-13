#pragma once

namespace constants
{

inline constexpr double func(double x) noexcept
{
    return x / ((3 * x + 4) * (3 * x + 4) * (3 * x + 4));
}

inline constexpr double x0 { -1.0 };
inline constexpr double xk { 1.0 };
inline constexpr double h1 { 0.5 };
inline constexpr double h2 { 0.25 };


} // namespace constants