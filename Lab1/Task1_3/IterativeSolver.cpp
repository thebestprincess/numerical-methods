#include "IterativeSolver.hpp"

#include <ranges>
#include <cmath>
#include <optional>
#include <algorithm>

namespace {

struct MatrixVectorStruct 
{
    Matrix A;
    std::vector<double> b;
};

auto make_permutation_vector(const Matrix& A) -> std::expected<std::vector<std::optional<size_t>>, std::string_view>
{
    const size_t N { A.get_rows() };
    std::vector<std::optional<size_t>> permutations(N, std::nullopt);

    for (size_t i { 0 }; i < N; ++i)
    {
        double sum { 0.0 };
        double mx_val { 0.0 };
        size_t mx_col { 0 };

        for (size_t j { 0 }; j < N; ++j)
        {
            double val { std::abs(A(i, j)) };
            sum += val;
            if (mx_val < val)
            {
                mx_val = val;
                mx_col = j;
            }
        }

        if (mx_val < sum - mx_val) 
            return std::unexpected("There is no dominant element in the row.");
        if (permutations[mx_col].has_value()) 
            return std::unexpected("It is impossible to transform the matrix into one with diagonal dominance."); 

        permutations[mx_col] = i;
    }

    return permutations;
}

auto make_diagonally_dominant(const Matrix& A, const std::vector<double>& b) -> std::expected<MatrixVectorStruct, std::string_view>
{
    auto expected_res { make_permutation_vector(A) };
    if (!expected_res.has_value()) return std::unexpected(expected_res.error());
    
    auto permutations { std::move(expected_res.value()) };
    
    const size_t N { A.get_rows() };
    Matrix new_A { N, N };
    std::vector<double> new_b(N);

    for (size_t new_i { 0 }; new_i < N; ++new_i)
    {
        size_t old_i { permutations[new_i].value() };

        new_b[new_i] = b[old_i];
        for (size_t j { 0 }; j < N; ++j) new_A(new_i, j) = A(old_i, j);
    }

    return MatrixVectorStruct{ std::move(new_A), std::move(new_b) };
}

MatrixVectorStruct get_equivalent_system(const Matrix& A, const std::vector<double>& b)
{
    size_t cols = A.get_cols();
    size_t rows = A.get_rows();
    MatrixVectorStruct sys { Matrix(rows, cols), std::vector<double>(b.size()) };
    
    for (size_t i { 0 }; i < rows; ++i)
    {
        for (size_t j { 0 }; j < cols; ++j)
        {
            if (i != j) sys.A(i, j) = -A(i, j) / A(i, i);
            else sys.A(i, j) = 0.0;
        }

        sys.b[i] = b[i] / A(i, i);
    }

    return sys;
}

double calculate_matrix_norm(const Matrix& alpha)
{
    const size_t N { alpha.get_rows() };
    double norm { 0.0 };
    
    for (size_t i { 0 }; i < N; ++i)
    {
        double row_sum { 0.0 };
        for (size_t j { 0 }; j < N; ++j)
        {
            row_sum += std::abs(alpha(i, j));
        }
        norm = std::max(norm, row_sum);
    }
    
    return norm;
}

double calculate_norm(const std::vector<double>& x_new, 
                      const std::vector<double>& x_old)
{
    double mx_diff { 0.0 };
    for (auto [new_val, old_val] : std::views::zip(x_new, x_old))
    {
        mx_diff = std::max(mx_diff, std::abs(new_val - old_val));
    }

    return mx_diff;
}

} // namespace

std::expected<IterativeResult, std::string_view> IterativeSolver::solve_simple_iterations(const Matrix& A, const std::vector<double>& b, double eps)
{
    if (!A.is_square()) return std::unexpected("The matrix must be square.");
    auto expected_res { make_diagonally_dominant(A, b) };
    if (!expected_res.has_value()) return std::unexpected(expected_res.error());

    auto [new_A, new_b] { expected_res.value() };
    auto [alpha, beta] { get_equivalent_system(new_A, new_b) };

    double alpha_norm { calculate_matrix_norm(alpha) };
    if (alpha_norm >= 1.0) return std::unexpected("The norm of the equivalent matrix alpha is >= 1. Sufficient convergence condition is not met.");

    double error_coef { alpha_norm / (1.0 - alpha_norm) };

    const size_t N { alpha.get_rows() }; 
    std::vector<double> old_x(N, 0.0);
    std::vector<double> new_x { beta };
    
    size_t iterations { 0 };
    do
    {
        old_x = new_x;
        
        for (size_t i { 0 }; i < N; ++i)
        {
            double sum { 0.0 };
            for (size_t j { 0 }; j < N; ++j)
            {
                sum += alpha(i, j) * old_x[j];
            }
            
            new_x[i] = beta[i] + sum; 
        }

        ++iterations;
    } while (error_coef * calculate_norm(new_x, old_x) >= eps);

    return IterativeResult{ std::move(new_x), iterations };
}

std::expected<IterativeResult, std::string_view> IterativeSolver::solve_seidel(const Matrix &A, const std::vector<double> &b, double eps)
{
    auto expected_res { make_diagonally_dominant(A, b) };
    if (!expected_res.has_value()) return std::unexpected(expected_res.error());

    auto [new_A, new_b] { expected_res.value() };
    auto [alpha, beta] { get_equivalent_system(new_A, new_b) };

    double alpha_norm { calculate_matrix_norm(alpha) };
    if (alpha_norm >= 1.0) return std::unexpected("The norm of the equivalent matrix alpha is >= 1. Sufficient convergence condition is not met.");

    double error_coef { alpha_norm / (1.0 - alpha_norm) };

    const size_t N { alpha.get_rows() }; 
    std::vector<double> old_x(N, 0.0);
    std::vector<double> x { beta };

    size_t iterations { 0 };
    do 
    {
        old_x = x;
        
        for (size_t i { 0 }; i < N; ++i)
        {
            double sum { 0.0 };
            for (size_t j { 0 }; j < N; ++j)
            {
                sum += alpha(i, j) * x[j];
            }
            
            x[i] = beta[i] + sum;
        }

        ++iterations;
    } while (error_coef * calculate_norm(x, old_x) >= eps);

    return IterativeResult{ std::move(x), iterations };
}
