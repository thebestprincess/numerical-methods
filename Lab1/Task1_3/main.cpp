#include "IterativeSolver.hpp"
#include "IOUtils.hpp"
#include "LUSolver.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <print>
#include <span>
#include <fstream>

int solver(const std::filesystem::path& file_path, double eps)
{
    std::ifstream file(file_path);
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    size_t num_systems { 0 };
    file >> num_systems;

    for (size_t i { 0 }; i < num_systems; ++i)
    {
        std::println("Test {}", i + 1);
        auto [A, b] = IOUtils::read_slae(file);

        auto sim_exp_result { IterativeSolver::solve_simple_iterations(A, b, eps)};
        if (!sim_exp_result.has_value())
        {
            std::println(stderr, "{}\n", sim_exp_result.error());
            continue;
        }
        IterativeResult sim_result { std::move(sim_exp_result.value()) };

        auto seidel_exp_result { IterativeSolver::solve_seidel(A, b, eps) }; 
        if (!seidel_exp_result.has_value())
        {
            std::println(stderr, "{}\n", seidel_exp_result.error());
            continue;
        }
        IterativeResult seidel_result { std::move(seidel_exp_result.value()) };

        
        auto decompose_option { LUSolver::decompose(A) };
        if (!decompose_option.has_value()) return 1;

        LUResult lu { std::move(decompose_option.value()) };
        std::vector<double> reference_x { LUSolver::solve(lu, b) };

        std::println("Eps: {}", eps);
        std::println("Simple iteration method: \nIterations: {}", sim_result.iterations);
        IOUtils::print_vector(sim_result.x);
        std::println("Seidel method:\nIterations: {}", seidel_result.iterations);
        IOUtils::print_vector(seidel_result.x);
        std::println("Reference results:");
        IOUtils::print_vector(reference_x);
        std::println("SIM-Siedel ratio: {:.{}f}",
            static_cast<double>(sim_result.iterations) / seidel_result.iterations,
            constants::precision);
        std::println();
    }

    return 0;
}

int main(int argc, char** argv)
{
    std::span<char*> args { argv, static_cast<size_t>(argc) };
    if (args.size() < 3)
    {
        std::println(stderr, "Usage: {} <path_to_input_file>", argv[0]);
        return 1;
    }

    std::string_view file_path_sv { args[1] };
    std::filesystem::path file_path { file_path_sv };

    const double eps { std::stod(args[2]) };

    int result { solver(file_path, eps) };
    if (result != 0) std::println("Something went wrong... Error: {}", result);

    return 0;
}