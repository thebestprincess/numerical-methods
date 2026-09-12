#include "IOUtils.hpp"
#include "Constants.hpp"
#include "LUSolver.hpp"

#include <fstream>
#include <print>
#include <iostream>

SystemInput IOUtils::read_slae(std::ifstream& file)
{
    size_t N { 0 };
    file >> N;

    Matrix A { N, N };
    std::vector<double> b(N);

    for (size_t i { 0 }; i < N; ++i)
    {
        for (size_t j { 0 }; j < N; ++j)
        {
            file >> A(i, j);
        }

        file >> b[i];
    }

    return { std::move(A), std::move(b) };
}

void IOUtils::print_matrix(const Matrix &m)
{
    const size_t rows { m.get_rows() };   
    const size_t cols { m.get_cols() };
    for (size_t i { 0 }; i < rows; ++i)
    {
        for (size_t j { 0 }; j < cols; ++j)
        {
            std::print("{:>{}.{}f}", m(i, j), constants::width, constants::precision);
        }
        std::println();
    }
}

void IOUtils::print_vector(const std::vector<double>& v)
{
    std::println("{::>{}.{}f}", v, constants::width, constants::precision);
}

void IOUtils::print_permutation_vector(const std::vector<size_t>& P_vec)
{
    Matrix P { LUResult::get_permutation_matrix(P_vec) };
    print_matrix(P);
}
