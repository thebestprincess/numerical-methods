#ifndef JACOBISOLVER_HPP
#define JACOBISOLVER_HPP

#include "Matrix.hpp"

#include <string_view>
#include <expected>
#include <vector>

struct JacobiResult
{
    Matrix eigenvectors;
    std::vector<double> eigenvalues;
    size_t iterations;

    Matrix get_lambda_matrix() const;
    static std::vector<double> get_eigenvalues(const Matrix& A);
};


class JacobiSolver final
{
public: 
    static std::expected<JacobiResult, std::string_view> solve(const Matrix& A, double eps);
};

#endif // JACOBISOLVER_HPP