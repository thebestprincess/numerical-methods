#ifndef ITERATIVESOLVER_HPP
#define ITERATIVESOLVER_HPP

#include "Matrix.hpp"

#include <expected>
#include <cstddef>
#include <vector>
#include <string_view>

struct IterativeResult
{
    std::vector<double> x;
    size_t iterations;
};

class IterativeSolver final
{
public:
    static std::expected<IterativeResult, std::string_view> solve_simple_iterations(
        const Matrix& A,
        const std::vector<double>& b,
        double eps);

    static std::expected<IterativeResult, std::string_view> solve_seidel(
        const Matrix& A,
        const std::vector<double>& b,
        double eps);
};


#endif // ITERATIVESOLVER_HPP
