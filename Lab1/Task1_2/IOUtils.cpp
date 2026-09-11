#include "IOUtils.hpp"
#include "Constants.hpp"

#include <ranges>
#include <print>

TriSystem IOUtils::read_slae(std::ifstream& file)
{
    size_t N { 0 };
    file >> N;

    std::vector<double> a(N, 0.0);
    std::vector<double> b(N, 0.0);
    std::vector<double> c(N, 0.0);
    std::vector<double> d(N, 0.0);
    for (size_t i { 0 }; i < N; ++i)
    {
        for (size_t j { 0 }; j < N; ++j)
        {
            double val { 0.0 };
            file >> val;

            if (i != 0 && j == i - 1) a[i] = val;
            else if (j == i) b[i] = val;
            else if (j == i + 1) c[i] = val;
        }

        file >> d[i];
    }

    return TriSystem{ std::move(a), std::move(b), std::move(c), std::move(d) };
}

void IOUtils::print_vector(const std::vector<double>& v)
{
    std::println("{::>{}.{}f}", v, constants::width, constants::precision);
}

void IOUtils::print_result_table(const TriResult& result)
{
    const size_t N { result.x.size() };
    if (N == 0) return;

    std::println("+{:^{}}|{:^{}}|{:^{}}+",
                 "P", constants::width,
                 "Q", constants::width,
                 "x", constants::width);
    
    for (size_t i { 0 }; i < N; ++i)
    {
        std::println("+{:->{}}+{:->{}}+{:->{}}+", 
                     "", constants::width, 
                     "", constants::width, 
                     "", constants::width);
        std::println("+{:^{}.{}f}|{:^{}.{}f}|{:^{}.{}f}+",
                     result.P[i], constants::width, constants::precision,
                     result.Q[i], constants::width, constants::precision,
                     result.x[i], constants::width, constants::precision);
        std::println("+{:->{}}+{:->{}}+{:->{}}+", 
                     "", constants::width, 
                     "", constants::width, 
                     "", constants::width);
    }
}
