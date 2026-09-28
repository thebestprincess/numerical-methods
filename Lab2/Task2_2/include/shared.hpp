#pragma once

#include <array>
#include <cstddef>

struct Result
{
    std::array<double, 2> x;
    size_t iterations;
};

namespace constants
{
    inline constexpr size_t precision { 4 };
    inline constexpr size_t width { 10 };
}