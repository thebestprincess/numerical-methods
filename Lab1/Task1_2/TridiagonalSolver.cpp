#include "TridiagonalSolver.hpp"
#include "Constants.hpp"

#include <ranges>
#include <cmath>

bool verify_stability(const TriSystem& sys)
{
    const size_t N { sys.d.size() };
    if (!N) return false;
    
    bool has_strict_inequality = false;
    for (size_t i { 0 }; i < N; ++i)
    {
        if (i > 0 && i < N - 1)
        {
            if (std::abs(sys.a[i]) < constants::eps || std::abs(sys.c[i]) < constants::eps)
            {
                return false;
            }
        }

        double lhs = std::abs(sys.b[i]);
        double rhs = std::abs(sys.a[i]) + std::abs(sys.c[i]);

        if (rhs - lhs > constants::eps) return false;
        if (lhs - rhs > constants::eps) has_strict_inequality = true;
    }

    return has_strict_inequality;
}

struct ForwardPassResult
{
    std::vector<double> P, Q;
};

ForwardPassResult forward_pass(const TriSystem& sys)
{
    const size_t N { sys.d.size() };

    ForwardPassResult result { std::vector<double>(N), std::vector<double>(N)};
    result.P[0] =  -sys.c[0] / sys.b[0];
    result.Q[0] = sys.d[0] / sys.b[0];
    for (size_t i { 1 }; i < N; ++i)
    {
        double denominator { sys.a[i] * result.P[i - 1] + sys.b[i] };
        result.P[i] = -sys.c[i] / denominator;
        result.Q[i] = (sys.d[i] - sys.a[i] * result.Q[i - 1]) / denominator;
    }

    return result;
}

std::vector<double> backward_pass(const std::vector<double>& P, const std::vector<double>& Q)
{
    const size_t N { P.size() };

    std::vector<double> x(N); 
    x[N - 1] = Q[N - 1];
    for (size_t i : std::views::iota(0uz, N - 1) | std::views::reverse)
    {
        x[i] = P[i] * x[i + 1] + Q[i];
    }

    return x;
}

std::expected<TriResult, std::string_view> TriSolver::solve(const TriSystem& sys)
{
    if (sys.d.empty()) return std::unexpected("System is empty.");
    if (!verify_stability(sys)) return std::unexpected("The system does not possess diagonal dominance.");

    auto [P, Q] = forward_pass(sys);
    std::vector<double> x = backward_pass(P, Q);

    return TriResult{ std::move(P), std::move(Q), std::move(x) };
}