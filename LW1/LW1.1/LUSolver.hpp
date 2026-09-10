#ifndef LUSOLVER_HPP
#define LUSOLVER_HPP

#include "Matrix.hpp"

#include <optional>

struct LUResult
{
    Matrix L;
    Matrix U;
    std::vector<size_t> P_vec;
    size_t swaps_count { 0 };
    LUResult(Matrix L, Matrix U, std::vector<size_t> P, size_t swaps_count);
    
    Matrix get_permutation_matrix() const;
    
    static Matrix get_permutation_matrix(const std::vector<size_t>& P);
};

class LUSolver final
{
public:
    static std::optional<LUResult> decompose(const Matrix& A);
    static std::vector<double> solve(const LUResult& lu, const std::vector<double>& b);
    static Matrix inverse(const LUResult& lu);
    static double determinant(const LUResult& lu);
};


#endif //LUSOLVER_HPP