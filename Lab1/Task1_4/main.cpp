#include "Constants.hpp"
#include "IOUtils.hpp"
#include "JacobiSolver.hpp"
#include "Matrix.hpp"

#include <filesystem>
#include <fstream>
#include <span>
#include <print>
#include <string>
#include <utility>
#include <ranges>

int solve(const std::filesystem::path& file_path, double eps)
{
    std::ifstream file { file_path };
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    size_t num_systems { 0 };
    file >> num_systems;

    for (size_t i { 0 }; i < num_systems; ++i)
    {
        std::println("Test {}", i + 1);

        Matrix A { IOUtils::read_matrix(file) };
        
        auto expected_res { JacobiSolver::solve(A, eps) }; 
        if (!expected_res.has_value())
        {
            std::println(stderr, "{}\n", expected_res.error());
            continue;
        }

        JacobiResult result { std::move(expected_res.value()) };

        Matrix& V { result.eigenvectors };
        Matrix L { result.get_lambda_matrix() };
   
        Matrix lhs = A * V;
        Matrix rhs = V * L;
        
        std::println("Eps: {}", eps);
        std::println("Eigenvalues: ");
        std::print("[");
        for (auto [index, val] : std::views::enumerate(result.eigenvalues))
            std::print("{:.{}f}{}", val, constants::precision, (index == result.eigenvalues.size() - 1) ? "" : ",  ");
        std::print("]\n");
        std::println("Matrix of natural vectors: ");
        IOUtils::print_matrix(V);

        if (Matrix::is_equal_to(lhs, rhs, eps)) std::println("Check A * V == V * L: Success");
        else std::println("Check A * V == V * L: Failed");
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
    
    int result { solve(file_path, eps) };
    if (result) std::println("Something went wrong... Error: {}", result);

    return 0;
}