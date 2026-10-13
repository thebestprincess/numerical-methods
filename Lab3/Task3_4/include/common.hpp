#pragma once

#include <array>

namespace constants
{

inline constexpr double eps {1e-9};
inline constexpr double x_star { 2.0 };
inline constexpr std::array<double, 5> x { 1.0, 1.5, 2.0, 2.5, 3.0 };
inline constexpr std::array<double, 5> y { 0.0, 0.40547, 0.69315, 0.91629, 1.0986 };

} // namespace::constants