#include "LUSolver.hpp"
#include "Constants.hpp"

#include <numeric>
#include <optional>
#include <utility>
#include <cmath>
#include <ranges>

LUResult::LUResult(Matrix L, Matrix U, std::vector<size_t> P,
                   size_t swaps_count): L { std::move(L) }, U{ std::move(U) },
                                        P_vec { std::move(P) }, swaps_count{swaps_count} {}

Matrix LUResult::get_permutation_matrix() const
{
    return get_permutation_matrix(P_vec);
}

Matrix LUResult::get_permutation_matrix(const std::vector<size_t>& P_vec)
{
    size_t N { P_vec.size() };
    Matrix P { N, N };
    for (size_t i{ 0 }; i < N; ++i)
    {
        P(i, P_vec[i]) = 1.0;
    }

    return P;
}

namespace {

std::optional<size_t> find_pivot_row(const Matrix& U, size_t current_step, double eps)
{
    double mx_abs { 0.0 };
    size_t total_rows { U.get_rows() };
    size_t pivot_row { current_step };

    for (size_t i { current_step }; i < total_rows; ++i)
    {
        double abs_elem { std::abs(U(i, current_step)) };
        if (mx_abs < abs_elem)
        {
            mx_abs = abs_elem;
            pivot_row = i;
        }
    }

    if (mx_abs < eps) return std::nullopt;

    return pivot_row;
}

} // namespace

std::optional<LUResult> LUSolver::decompose(const Matrix& A)
{
    if (!A.is_square()) return std::nullopt;

    const size_t N { A.get_cols() };
    std::vector<size_t> permutations(N);
    std::iota(permutations.begin(), permutations.end(), 0);
    
    Matrix U { A };
    Matrix L { Matrix::identity(N) };
    size_t swaps_count { 0 };

    for (size_t i { 0 }; i < N; ++i)
    {
        auto pivot_opt { find_pivot_row(U, i, constants::eps) };
        if (!pivot_opt.has_value()) return std::nullopt;
        size_t pivot_row_idx { pivot_opt.value() };

        if (pivot_row_idx != i)
        {
            U.swap_rows(pivot_row_idx, i);
            ++swaps_count;

            std::swap(permutations[pivot_row_idx], permutations[i]);

            for (size_t j { 0 }; j < i; ++j)
            {
                std::swap(L(i, j), L(pivot_row_idx, j));
            }
        }

        for (size_t j { i + 1 }; j < N; ++j)
        {
            double mu { U(j, i) / U(i, i) };
            
            L(j, i) = mu;

            for (size_t k { i }; k < N; ++k)
            {
                U(j, k) -= mu * U(i, k);
            }
        }
    }

    return LUResult{ std::move(L), std::move(U), std::move(permutations), swaps_count };
}

std::vector<double> LUSolver::solve(const LUResult& lu, const std::vector<double>& b)
{
    const size_t N { b.size() };
    std::vector<double> b_permutated(N);
    for (size_t i { 0 }; i < N; ++i) b_permutated[i] = b[lu.P_vec[i]];

    std::vector<double> z(N);
    const Matrix& L { lu.L };
    for (size_t row { 0 }; row < N; ++row)
    {
        double sum { 0.0 };
        for (size_t col { 0 }; col < row; ++col)
        {
            sum += z[col] * L(row, col);
        }

        z[row] = b_permutated[row] - sum;
    }

    const Matrix& U { lu.U };
    std::vector<double> x(N);
    for (auto row : std::views::iota(0uz, N) | std::views::reverse)
    {
        double sum { 0.0 };
        for (size_t col { row + 1 }; col < N; ++col)
        {
            sum += U(row, col) * x[col];  
        }

        x[row] = (z[row] - sum) / U(row, row); 
    }

    return x;
}

Matrix LUSolver::inverse(const LUResult &lu)
{
    const size_t N { lu.L.get_rows() };
    Matrix inverse_A { N, N };
    for (size_t col { 0 }; col < N; ++col)
    {
        std::vector<double> e(N);
        e[col] = 1.0;

        auto x = solve(lu, e);
        for (size_t row { 0 }; row < N; ++row)
        {
            inverse_A(row, col) = x[row];
        }
    }

    return inverse_A;
}

double LUSolver::determinant(const LUResult &lu)
{
    const Matrix& U { lu.U };
    const size_t N { U.get_rows() };

    double mult { 1.0 };
    for (size_t i { 0 }; i < N; ++i) mult *= U(i, i);
    
    double sign { (lu.swaps_count & 1) ? -1.0 : 1.0 };
    return sign * mult;
}
