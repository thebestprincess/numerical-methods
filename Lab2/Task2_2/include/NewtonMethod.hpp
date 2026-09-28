#pragma once

#include "concepts.hpp"
#include "shared.hpp"

#include <cstddef>
#include <cstdlib>
#include <expected>
#include <mdspan>
#include <ranges>
#include <string_view>


namespace
{
using Matrix2x2 = std::mdspan<double, std::extents<size_t, 2, 2>>;
constexpr double calc_determinant_2x2(Matrix2x2 matrix)
{
    return matrix[0, 0] * matrix[1, 1] - matrix[0, 1] * matrix[1, 0]; 
}

constexpr double mx_abs_diff(const auto& prev_x, const auto& x, double eps)
{
    double mx_abs_diff { 0.0 };
    for (auto&& [prev, curr] : std::views::zip(prev_x, x))
    {
        mx_abs_diff = std::max(mx_abs_diff, std::abs(curr - prev)); 
    }

    return mx_abs_diff;
}

constexpr bool termination_condition(const auto& prev_x, const auto& x, double eps)
{
    if (mx_abs_diff(prev_x, x, eps) >= eps) return false; 
    return true;
}

template <concepts::SysEq S>
constexpr void update_matrices(Matrix2x2 A1, Matrix2x2 A2, Matrix2x2 J, const auto& x)
{
    A1[0, 0] = S::f1(x);
    A1[0, 1] = S::f1dx2(x);
    A1[1, 0] = S::f2(x);
    A1[1, 1] = S::f2dx2(x);

    A2[0, 0] = S::f1dx1(x);
    A2[0, 1] = S::f1(x);
    A2[1, 0] = S::f2dx1(x);
    A2[1, 1] = S::f2(x);

    J[0, 0] = S::f1dx1(x);
    J[0, 1] = S::f1dx2(x);
    J[1, 0] = S::f2dx1(x);
    J[1, 1] = S::f2dx2(x);
}

} // namespace

template <concepts::SysEq S>
constexpr std::expected<Result, std::string_view> newton_method(double eps, size_t max_iter = 1000)
{
    const size_t N { S::dimension * S::dimension }; 

    std::array<double, N> A1_data {};
    std::array<double, N> A2_data {};
    std::array<double, N> J_data {};
    
    Matrix2x2 A1 {A1_data.data()};
    Matrix2x2 A2 {A2_data.data()};
    Matrix2x2 J {J_data.data()};

    typename S::XVec prev_x {};
    typename S::XVec x { S::x0 };
    size_t iterations { 0 };
    do
    {
        if (iterations >= max_iter)
            return std::unexpected("Maximum iterations reached without convergence.");
        
        update_matrices<S>(A1, A2, J, x);
        prev_x = x;

        const double det_A1 = calc_determinant_2x2(A1);
        const double det_A2 = calc_determinant_2x2(A2);
        const double det_J = calc_determinant_2x2(J);

        if (std::abs(det_J) < 1e-15)
            return std::unexpected("Jacobian determinant is zero.");

        x[0] = prev_x[0] - det_A1 / det_J;
        x[1] = prev_x[1] - det_A2 / det_J;

        ++iterations;
    } while(!termination_condition(prev_x, x, eps));

    return Result{x, iterations};
}
