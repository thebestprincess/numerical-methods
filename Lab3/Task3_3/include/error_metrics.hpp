#pragma once

#include <span>
#include <cstddef>

struct ErrorMetrics
{
    double sse; // Sum of Squared Errors
    double mse; // Mean Squared Error
};

template <std::size_t Degree, std::size_t N>
[[nodiscard]]
constexpr ErrorMetrics compute_errors(
    std::span<const double, N> x, 
    std::span<const double, N> y, 
    std::span<const double, Degree + 1> coeffs
) noexcept
{
    double sse { 0.0 };
    for (size_t i { 0 }; i < N; ++i)
    {
        double predicted { 0.0 }; 
        double x_pow { 1.0 };
        for (size_t j { 0 }; j <= Degree; ++j)
        {
            predicted += coeffs[j] * x_pow;
            x_pow *= x[i];
        }

        const double diff { predicted - y[i] };
        sse += diff * diff;
    }

    return ErrorMetrics{ sse, sse / N };
}