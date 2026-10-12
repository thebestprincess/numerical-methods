#pragma once

#include <array>
#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <span>
#include <expected>

template <size_t M>
[[nodiscard]]
constexpr auto solve_gauss(
    std::array<std::array<double, M>, M> A,
    std::array<double, M> B
) noexcept
    -> std::expected<std::array<double, M>, std::string_view> 
{
    for (size_t i { 0 }; i < M; ++i)
    {
        size_t pivot { i };
        for (size_t j { i + 1 }; j < M; ++j)
        {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i]))
            {
                pivot = j;
            }
        }
        
        if (std::abs(A[pivot][i]) < 1e-12) [[unlikely]]
        {
            return std::unexpected("Singular matrix encountered in Gaussian elimination.");
        }
        
        std::swap(A[i], A[pivot]);
        std::swap(B[i], B[pivot]);
        
        for (size_t j { i + 1 }; j < M; ++j)
        {
            const double factor { A[j][i] / A[i][i] };
            for (size_t k { i }; k < M; ++k)
            {
                A[j][k] -= factor * A[i][k];
            }

            B[j] -= factor * B[i];
        }
    }
    
    std::array<double, M> x{};
    for (size_t i { M }; i-- > 0; )
    {
        x[i] = B[i];
        for (size_t j { i + 1 }; j < M; ++j)
        {
            x[i] -= A[i][j] * x[j];
        }

        x[i] /= A[i][i];
    }
    
    return x;
}

// Builds the normal equation system (X^T * X) * A = X^T * Y and solves it
template <size_t Degree, size_t N>
[[nodiscard]]
constexpr auto build_polynomial(
    std::span<const double, N> x,
    std::span<const double, N> y
) noexcept
    -> std::expected<std::array<double, Degree + 1>, std::string_view>
{
    constexpr size_t M { Degree + 1 };
    std::array<std::array<double, M>, M> A{};
    std::array<double, M> B{};
    
    for (size_t i { 0 }; i < N; ++i)
    {
        double x_pow_row { 1.0 };
        for (size_t row { 0 }; row < M; ++row)
        {
            B[row] += y[i] * x_pow_row;
            
            double x_pow_col { x_pow_row };
            for (size_t col { 0 }; col < M; ++col)
            {
                A[row][col] += x_pow_col;
                x_pow_col *= x[i];
            }

            x_pow_row *= x[i];
        }
    }

    return solve_gauss<M>(A, B);
}