#include "IOUtils.hpp"
#include "LUSolver.hpp"
#include "Constants.hpp"

#include <filesystem>
#include <print>

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
    
    try
    {
        auto [A, b] = IOUtils::read_slae(file_path);
        
        auto decompose_option { LUSolver::decompose(A) };
        if (!decompose_option.has_value()) return 2;

        LUResult lu { std::move(decompose_option.value()) };
        std::vector<double> x { LUSolver::solve(lu, b) };

        std::println("L: ");
        IOUtils::print_matrix(lu.L);
        std::println();

        std::println("U: ");
        IOUtils::print_matrix(lu.U);
        std::println();

        std::println("L * U:");
        IOUtils::print_matrix(lu.L * lu.U);
        std::println();

        std::println("vector x: ");
        IOUtils::print_vector(x);
        std::println();

        std::println("Inverse A:");
        Matrix inverse_A { LUSolver::inverse(lu) };
        IOUtils::print_matrix(inverse_A);
        std::println();

        std::println("Determinant: {:.{}f}", LUSolver::determinant(lu), constants::precision);
        std::println();

        std::println("A * inv_A check: ");
        Matrix E_check { A * inverse_A };
        Matrix E { Matrix::identity(A.get_rows()) };
        if (E_check == E) std::println("E == A * A^-1");
        else std::println("E != A * A^-1");
        std::println();

        std::println("L * U = P * A check: ");
        Matrix P { LUResult::get_permutation_matrix(lu.P_vec) };
        if (lu.L * lu.U == P * A) std::println("L * U == P * A");
        else std::println("L * U != P * A");
    }
    catch(std::exception& e)
    { 
        std::println(stderr, "Fatal error: {}", e.what());
        return 3;
    }

    return 0;
}