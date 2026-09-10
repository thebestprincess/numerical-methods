#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix final
{
    std::vector<double> data_;
    size_t cols_;
    size_t rows_;
    
public:
    Matrix(size_t rows, size_t cols);
    
    size_t get_cols() const;
    size_t get_rows() const;
    bool is_square() const;
    void swap_rows(size_t row1, size_t row2);

    static Matrix identity(size_t n);
    
    double operator()(size_t row, size_t col) const;
    double& operator()(size_t row, size_t col);
    
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend bool operator==(const Matrix& lhs, const Matrix& rhs);

private:
    bool is_equal_to(const Matrix& rhs, double eps) const;
};

#endif // MATRIX_LW1_HPP
