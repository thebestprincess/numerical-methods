#include "Matrix.hpp"
#include "Constants.hpp"

#include <stdexcept>
#include <cmath>
#include <utility>

Matrix::Matrix(size_t rows, size_t cols): data_(rows * cols, 0.0), rows_{rows}, cols_{cols} {}

size_t Matrix::get_cols() const
{
    return cols_;
}

size_t Matrix::get_rows() const
{
    return rows_;
}

bool Matrix::is_square() const
{
    return rows_ == cols_;
}

bool Matrix::is_symmetric() const
{
    if (!is_square()) return false;

    const size_t n = rows_;
    for (size_t row { 0 }; row < n; ++row)
    {
        for (size_t col { row + 1 }; col < n; ++col)
        {
            if (std::abs(data_[row * n + col] - data_[col * n + row]) > constants::eps)
                return false;
        }
    }

    return true;
}

void Matrix::swap_rows(size_t row1, size_t row2)
{
    if (row1 == row2) return;

    for (size_t i { 0 }; i < cols_; ++i)
    {
        std::swap(data_[row1 * cols_ + i], data_[row2 * cols_ + i]);
    }
}

Matrix Matrix::identity(size_t n)
{
    Matrix id_mat { n, n };
    for (size_t i { 0 }; i < n; ++i)
    {
        id_mat(i, i) = 1.0;
    }

    return id_mat;
}

double Matrix::operator()(size_t row, size_t col) const 
{ 
    return data_[row * cols_ + col]; 
}

double& Matrix::operator()(size_t row, size_t col) 
{ 
    return data_[row * cols_ + col]; 
}

bool Matrix::is_equal_to(const Matrix& lhs, const Matrix &rhs, double eps) 
{
    if (lhs.cols_ != rhs.cols_ || lhs.rows_ != rhs.rows_) return false;
    
    const size_t N { lhs.cols_ };
    for (size_t i { 0 }; i < N; ++i)
    {
        for (size_t j { 0 }; j < N; ++j)
        {
            double abs_dif { std::abs(lhs(i, j) - rhs(i, j)) };
            if (abs_dif > eps) return false;
        }
    }

    return true;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    if (lhs.cols_ != rhs.rows_) 
        throw std::invalid_argument("Matrix dimensions do not match for multiplication");

    Matrix result { lhs.rows_, rhs.cols_ };
    for (size_t i { 0 }; i < lhs.rows_; ++i)
    {
        for (size_t j { 0 }; j < rhs.cols_; ++j)
        {
            double sum { 0.0 };
            for (size_t k { 0 }; k < lhs.cols_; ++k)
            {
                sum += lhs(i, k) * rhs(k, j); 
            }

            result(i, j) = sum;
        }
    }

    return result;
}

bool operator==(const Matrix &lhs, const Matrix &rhs)
{
    return Matrix::is_equal_to(lhs, rhs, constants::eps);
}
