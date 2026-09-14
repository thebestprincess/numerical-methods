#include "QRSolver.hpp"
#include "Constants.hpp"
#include "Matrix.hpp"

#include <complex>
#include <cmath>

namespace {

double sign(double x) { return (x >= 0.0) ? 1.0 : -1.0; }

std::vector<double> make_v(const Matrix& R, size_t i, double norm_b)
{
    const size_t N { R.get_rows() };

    std::vector<double> v(N, 0.0);
    v[i] = R(i, i) + sign(R(i, i)) * norm_b;
    for (size_t j { i + 1 }; j < N; ++j) v[j] = R(j, i);

    return v;
}

double make_norm(const Matrix& R, size_t i)
{
    const size_t N { R.get_rows() };

    double norm_b_sq { 0.0 };
    for (size_t j { i }; j < N; ++j) norm_b_sq += R(j, i) * R(j, i);

    return std::sqrt(norm_b_sq);
}

double scalar_product(std::vector<double>& v1, std::vector<double>& v2, size_t i)
{
    size_t size { v1.size() };
    
    double product { 0.0 };
    for (size_t j { i }; j < size; ++j) product += v1[j] * v2[j];

    return product;
}

bool solve_characteristic_equation(const Matrix& Ak, size_t i, std::vector<std::complex<double>>& eigenvalues)
{
    double a11 { Ak(i, i) };
    double a12 { Ak(i, i + 1) };
    double a21 { Ak(i + 1, i) };
    double a22 { Ak(i + 1, i + 1) };
    
    double tr { a11 + a22 };
    double det { a11 * a22 - a12 * a21 };
    double D { tr * tr - 4.0 * det };
    
    if (D <= constants::eps)
    {
        double real_part { tr / 2.0 };
        double imag_part { std::sqrt(-D) / 2.0 };
        eigenvalues.push_back(std::complex<double>(real_part, imag_part));
        eigenvalues.push_back(std::complex<double>(real_part, -imag_part));
    }
    else return false;

    return true;
}

}



std::expected<QRDecompResult, std::string_view> QRSolver::decompose(const Matrix& A)
{
    if (!A.is_square()) return std::unexpected("The matrix must be square.");

    const size_t N { A.get_rows() };
    Matrix R { A };
    Matrix Q { Matrix::identity(N) };

    for (size_t i { 0 }; i < N - 1; ++i)
    {
        double norm_b { make_norm(R, i) };
        if (norm_b <= constants::eps) continue;

        std::vector<double> v { make_v(R, i, norm_b) };

        double v_norm_sq { scalar_product(v, v, i) };

        Matrix H { Matrix::identity(N) };
        if (v_norm_sq > constants::eps)
        {
            for (size_t row { i }; row < N; ++row)
            {
                for (size_t col { i }; col < N; ++col)
                {
                    double outer_product = v[row] * v[col];
                    H(row, col) -= 2.0 * outer_product / v_norm_sq;
                }
            }
        }

        R = H * R;
        Q = Q * H;
    }

    return QRDecompResult{ std::move(Q), std::move(R) };    
}

std::expected<QREigenResult, std::string_view> QRSolver::find_eigenvalues(const Matrix& A, double eps)
{
    if (!A.is_square()) return std::unexpected("The matrix must be square.");

    const size_t N { A.get_rows() };
    Matrix Ak { A };
    
    size_t iterations { 0 };
    size_t mx_itertaions { 10000 };
    bool converged { false }; 
    std::vector<std::complex<double>> eigenvalues(N, 0.0);
    Matrix Q { N, N };
    Matrix R { N, N };

    while (!converged && iterations < mx_itertaions)
    {
        auto decomp_res { decompose(Ak) };
        if (!decomp_res) return std::unexpected(decomp_res.error());

        R = decomp_res->R;
        Q = decomp_res->Q;
        Ak = R * Q;
        
        converged = true;
        eigenvalues.clear();
        
        for (size_t i { 0 }; i < N;)
        {
            if (i == N - 1 || std::abs(Ak(i + 1, i)) <= eps)
            {
                eigenvalues.push_back(Ak(i, i));
                ++i; 
            } 
            else 
            {
                if (i == N - 2 || std::abs(Ak(i + 2, i + 1)) <= eps)
                {
                    auto result { solve_characteristic_equation(Ak, i, eigenvalues) };
                    if (!result)
                    {
                        converged = false;
                        break;
                    }

                    i += 2;
                }
                else
                {
                    converged = false;
                    break;
                }
            }
        }

        ++iterations;
    }

    if (!converged) std::unexpected("The QR algorithm did not converge within the specified number of iterations.");

    return QREigenResult{ std::move(eigenvalues), std::move(Q), std::move(R), iterations };
}