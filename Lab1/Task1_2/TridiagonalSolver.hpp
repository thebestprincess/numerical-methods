#ifndef TRIDIAGONAL_SOLVER_HPP
#define TRIDIAGONAL_SOLVER_HPP

#include <vector>
#include <optional>
#include <expected>
#include <string_view>

struct TriSystem
{
    std::vector<double> a;  // Lower diagonal
    std::vector<double> b;  // Middle diagonal
    std::vector<double> c;  // Upper diagonal 
    std::vector<double> d; 
};

struct TriResult
{
    std::vector<double> P;
    std::vector<double> Q;
    std::vector<double> x;
};

class TriSolver final
{
public:
    static std::expected<TriResult, std::string_view> solve(const TriSystem& sys);
};




#endif // TRIDIAGONAL_SOLVER_HPP
