#pragma once

#include <array>

namespace constants
{

inline constexpr double eps { 1e-9 };
inline constexpr double x_star { 1.5 };
inline constexpr std::array<double, 5> x { 0.0, 0.9, 1.8, 2.7, 3.6 }; 
inline constexpr std::array<double, 5> f { 0.0, 0.36892, 0.85408, 1.7856, 6.3138};

} // namespace constants