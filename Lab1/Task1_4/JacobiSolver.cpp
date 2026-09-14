#include "JacobiSolver.hpp"

#include <cmath>
#include <expected>

namespace {

struct MaxElemInfo
{
    size_t row;
    size_t col;
    double value;
};

MaxElemInfo find_maximum_elem(const Matrix& A)
{
    if (A.get_cols() == 1) return { 0, 0, A(0, 0)};

    MaxElemInfo mx_elem{ 0, 1, A(0, 1) };
    const size_t N { A.get_cols() }; 
    for (size_t row { 0 }; row < N; ++row)
    {
        for (size_t col { row + 1 }; col < N; ++col)
        {
            double val { A(row, col) };
            if (std::abs(mx_elem.value) < std::abs(val))
            {
                mx_elem.value = val;
                mx_elem.col = col;
                mx_elem.row = row;
            }
        }
    }

    return mx_elem;
}

bool condition_of_smallnes(const Matrix& A, double eps)
{
    double sum { 0.0 };
    const size_t N { A.get_cols() }; 
    for (size_t row { 0 }; row < N; ++row)
    {
        for (size_t col { row + 1 }; col < N; ++col)
        {
            double val { A(row, col) };
            sum += val * val;
        }
    }

    double sqrt_sum { std::sqrt(sum) };

    if (sqrt_sum < eps) return true;
    return false;
}

}


Matrix JacobiResult::get_lambda_matrix() const
{
    if (eigenvalues.empty()) return Matrix{0, 0};
    
    const size_t N { eigenvalues.size() }; 
    Matrix lambda_matrix { N, N };
    for (size_t i { 0 }; i < N; ++i) lambda_matrix(i, i) = eigenvalues[i];

    return lambda_matrix; 
}

std::vector<double> JacobiResult::get_eigenvalues(const Matrix& A)
{
    if (!A.is_square()) return {};

    const size_t N { A.get_cols() };
    std::vector<double> result(N, 0.0);
    for (size_t i { 0 }; i < N; ++i) result[i] = A(i, i);

    return result;
}

std::expected<JacobiResult, std::string_view> JacobiSolver::solve(const Matrix& A, double eps)
{
    if (!A.is_square()) return std::unexpected("The matrix must be square.");
    if (!A.is_symmetric()) return std::unexpected("The matrix must be symmetric.");

    const size_t N { A.get_rows() }; 
    Matrix A_k { A };
    Matrix V { Matrix::identity(N) };
    size_t iterations { 0 };

    do
    {
        auto [row, col, mxvl] { find_maximum_elem(A_k) };

        double a_ii { A_k(row, row) };
        double a_jj { A_k(col, col) };

        double phi { 0.5 * std::atan2(2.0 * mxvl, a_ii - a_jj) };
        double c { std::cos(phi) };
        double s { std::sin(phi) };

        A_k(row, row) = c * c * a_ii + 2 * c * s * A_k(row, col) + s * s * a_jj;
        A_k(col, col) = s * s * a_ii - 2 * c * s * A_k(row, col) + c * c * a_jj;
        A_k(row, col) = A_k(col, row) = 0;

        for (size_t k { 0 }; k < N; ++k)
        {
            if (k != row && k != col)
            {
                double a_ik { A_k(row, k) };
                double a_jk { A_k(col, k) };
                A_k(k, row) = A_k(row, k) = c * a_ik + s * a_jk;
                A_k(k, col) = A_k(col, k) = -s * a_ik + c * a_jk;
            }

            double v_ki { V(k, row) };
            double v_kj { V(k, col) };
            V(k, row) = c * v_ki + s * v_kj;
            V(k, col) = -s * v_ki + c * v_kj;
        }

        ++iterations;
    } while (!condition_of_smallnes(A_k, eps));

    return JacobiResult{ std::move(V), JacobiResult::get_eigenvalues(A_k), iterations};
}