#pragma once

#include <span>
#include <ranges>

class LagrangePolynomial
{
    std::span<const double> x_;
    std::span<const double> y_;

public:
    constexpr LagrangePolynomial(
        std::span<const double> x,
        std::span<const double> y
    ) noexcept
    : x_(x), y_(y) {} 

    [[nodiscard]]
    constexpr double operator()(double x) const noexcept
    {
        double total_sum { 0.0 };

        for (const auto [i, y_val] : std::views::enumerate(y_))
        {
            double influence_polynomial { 1.0 };    
            for (const auto [j, x_val] : std::views::enumerate(x_))
            {
                if (i != j) [[likely]]
                {
                    influence_polynomial *= (x - x_val) / (x_[i] - x_val);
                }
            }

            total_sum += y_val * influence_polynomial;
        }

        return total_sum;
    }
};