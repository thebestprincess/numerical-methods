#include "NewtonMethod.hpp"
#include "SimpIterMethod.hpp"
#include "SystemEquations.hpp"
#include "shared.hpp"

#include <cstddef>
#include <span>
#include <print>
#include <ranges>
#include <string>

void print_array(const auto& array)
{
    std::print("[");
    for (const auto&& [index, el] : array | std::views::enumerate)
    {
        std::print("{:>{}.{}f}", el, constants::width, constants::precision);
        if (index != array.size() - 1) std::print(",");
    }
    std::println("]");
}

int main(int argc, char** argv)
{
    const std::span<char*> args { argv, static_cast<size_t>(argc) };
    if (args.size() < 2) return 1;

    const double eps { std::stod(args[1]) };

    {
        std::println("Newton Method:");
        const auto result { newton_method<SystemEquations>(eps) };
        if (!result.has_value())
        {
            std::println("{}", result.error());    
            return 2;
        }

        std::print("x: ");
        print_array(result->x);
        std::println("iterations: {}\n", result->iterations);
    }

    {
        std::println("Simple Iterations Method:");
        const auto result { simple_iteration_method<SystemEquations>(eps) };
        if (!result.has_value())
        {
            std::println("{}", result.error());    
            return 3;
        }

        std::print("x: ");
        print_array(result->x);
        std::println("iterations: {}\n", result->iterations);
    }

    return 0;
}