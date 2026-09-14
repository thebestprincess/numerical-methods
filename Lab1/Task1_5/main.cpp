#include "Constants.hpp"
#include "QRSolver.hpp"
#include "IOUtils.hpp"
#include "Matrix.hpp"

#include <filesystem>
#include <fstream>
#include <span>
#include <print>
#include <string>
#include <utility>

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
        
        auto expected_res { QRSolver::find_eigenvalues(A, eps) }; 
        if (!expected_res.has_value())
        {
            std::println(stderr, "{}\n", expected_res.error());
            continue;
        }

        QREigenResult result { std::move(expected_res.value()) };
        
        std::println("A(0): ");
        IOUtils::print_matrix(A);
        std::println("Q({}): ", result.iterations);
        IOUtils::print_matrix(result.Q);
        std::println("R({}): ", result.iterations);
        IOUtils::print_matrix(result.R);
        std::println("Q * R: ");
        IOUtils::print_matrix(result.Q * result.R);
        for (const auto& val : result.eigenvalues) 
            std::println("({:.{}f}, {:.{}f})",
                         val.real(), constants::precision, 
                         val.imag(), constants::precision);
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