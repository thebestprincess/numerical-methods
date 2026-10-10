#pragma once

#include <span>
#include <stdexcept>
#include <vector>
#include <ranges>

class NewtonPolynomial
{
    std::span<const double> x_;
    std::span<const double> y_;
    std::vector<double> coefs;

    constexpr void calc_divided_diff(
        std::span<const double> y
    ) noexcept {
        const size_t N { x_.size() };

        for (size_t len { 1 }; len < N; ++len) 
        {
            for (size_t i { N - 1 }; i >= len; --i)
            {
                coefs[i] = (coefs[i] - coefs[i - 1]) / (x_[i] - x_[i - len]);
            }
        }
    }

public:
    constexpr NewtonPolynomial(
        std::span<const double> x,
        std::span<const double> y
    ) : x_(x), y_(y), coefs(std::from_range, y)
    {
        if (x.empty() || x.size() != y.size()) 
            throw std::invalid_argument("invalid argument NewtonPolynomial() constructor");
        calc_divided_diff(y);
    }

    [[nodiscard]]
    constexpr double operator()(
        double x
    ) const noexcept
    {
        if (coefs.empty()) return 0.0;

        const size_t N { coefs.size() };
        double accumulated_amount { coefs.back() };

        for (size_t i { N - 1 }; i > 0; --i)
        {
            accumulated_amount = coefs[i - 1] + (x - x_[i - 1]) * accumulated_amount;
        }

        return accumulated_amount;
    }

    [[nodiscard]]
    constexpr std::span<const double> get_coefs() const noexcept
    {
        return coefs;
    }
};

