#include "IOUtils.hpp"
#include "Constants.hpp"
#include "TridiagonalSolver.hpp"

#include <filesystem>
#include <print>
#include <fstream>

int thomas_method(const std::filesystem::path& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    size_t num_systems { 0 };
    file >> num_systems;

    for (size_t i { 0 }; i < num_systems; ++i)
    {
        std::println("Test {}", i + 1);
        try
        {
            TriSystem ts { IOUtils::read_slae(file) };

            auto solve_option { TriSolver::solve(ts) };
            if (!solve_option.has_value())
            {
                std::println("The sufficient condition for the stability of the Thomas method is not met.\n");
                continue;
            }

            TriResult result { std::move(solve_option.value()) };

            std::println("Vector x: ");
            IOUtils::print_vector(result.x);
            std::println();

            std::println("P&Q&x Table:");
            IOUtils::print_result_table(result);
            std::println();
        }
        catch(std::exception& e)
        { 
            std::println(stderr, "Fatal error: {}", e.what());
            return 2;
        }
        
        std::println();
    }

    return 0;
}

int main(int argc, char** argv)
{
    std::span<char*> args { argv, static_cast<size_t>(argc) };
    if (args.size() < 2)
    {
        std::println(stderr, "Usage: {} <path_to_input_file>", argv[0]);
        return 1;
    }

    std::string_view file_path_sv { args[1] };
    std::filesystem::path file_path { file_path_sv };
    
    int result = thomas_method(file_path);
    if (result != 0) std::println("Something went wrong... Error: {}", result);

    return 0;
}