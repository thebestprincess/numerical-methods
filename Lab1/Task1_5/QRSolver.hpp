#ifndef QRSOLVER_HPP
#define QRSOLVER_HPP

#include "Matrix.hpp"

#include <complex>
#include <expected>
#include <string_view>

struct QRDecompResult
{
    Matrix Q;
    Matrix R;
};

struct QREigenResult
{
    std::vector<std::complex<double>> eigenvalues; 
    Matrix Q;
    Matrix R;
    size_t iterations;
};

class QRSolver final
{
public:
    static std::expected<QRDecompResult, std::string_view> decompose(const Matrix& A);
    static std::expected<QREigenResult, std::string_view> find_eigenvalues(const Matrix& A, double eps);
};



#endif // QRSOLVER_HPP