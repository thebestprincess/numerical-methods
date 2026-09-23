#include "concepts.hpp"
#include "constants.hpp"
#include "FuncDeriv.hpp"
#include "NewtonMethod.hpp"
#include "SimpIterMethod.hpp"

#include <print>
#include <span>
#include <string>

template<concepts::FuncDeriv Func>
bool print_newton_result(double eps)
{
    const auto expected_result { newton_method<Func>(eps) };
    if (!expected_result)
    {
        std::println("{}", expected_result.error());
        return false;
    }

    const auto& result { expected_result.value() };

    std::println("Newton Method: ");
    std::println("x: {:.{}f}", result.x, constants::precision);
    std::println("iterations: {}\n", result.iterations);

    return true;
}

int newton(double eps)
{
    if (!print_newton_result<Func>(eps)) return 1;
    return 0;
}

template<concepts::FuncDeriv Func>
bool print_simp_iter_result(double eps)
{
    const auto expected_result { simp_iter_method<Func>(eps) };
    if (!expected_result)
    {
        std::println("{}", expected_result.error());
        return false;
    }

    const auto& result { expected_result.value() };

    std::println("Simple Iterations Method: ");
    std::println("x: {:.{}f}", result.x, constants::precision);
    std::println("iterations: {}\n", result.iterations);

    return true;
}


int simp_iter(double eps)
{
    if (!print_simp_iter_result<Func>(eps)) return 2;
    return 0;
}

int main(int argc, char** argv)
{
    const std::span<char*> args { argv, static_cast<size_t>(argc) };
    if (args.size() < 2)
    {
        std::println("You did not specify the precision.");
        return 1;
    }

    const double eps { std::stod(args[1]) };
    if (newton(eps)) return 1;
    if (simp_iter(eps)) return 2;

    return 0;
}