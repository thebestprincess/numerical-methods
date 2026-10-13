#include "common.hpp"
#include "differentiation.hpp"

#include <print>

void print_initial_table(
    std::span<const double> x,
    std::span<const double> y
) noexcept
{
    std::println("Initial Data:");
    std::println("{:>3} | {:>8} | {:>8}", "i", "x[i]", "y[i]");
    std::println("{:-^19}", "");
    for (size_t i = 0; i < x.size(); ++i)
    {
        std::println("{:3} | {:8.5f} | {:8.5f}", i, x[i], y[i]);
    }
    std::println();
}

void print_derivative_results(
    std::span<const double> x,
    std::span<const double> y,
    const double x_star
) noexcept
{
    auto result = math::calculate_derivatives(x, y, x_star);
    if (!result.has_value()) [[unlikely]]
    {
        std::print("{}\n", math::error_to_string(result.error()));
        return;
    }

    const size_t start_idx = result->used_start_index;
    
    std::println("Nodes Used for Approximation:");
    std::println("{:>3} | {:>8} | {:>8}", "i", "x[i]", "y[i]");
    std::println("{:-^19}", "");
    
    for (size_t i { start_idx }; i <= start_idx + 2; ++i)
    {
        std::println("{:3} | {:8.5f} | {:8.5f}", i, x[i], y[i]);
    }
    std::println();

    std::println("Differentiation Results");
    std::println("y'(X*)  = {:.5f}", result->first_derivative);
    std::println("y''(X*) = {:.5f}", result->second_derivative);
}

int main()
{
    const std::span<const double> x_span { constants::x };
    const std::span<const double> y_span { constants::y };
    const double target_x = constants::x_star;

    print_initial_table(x_span, y_span);

    std::println("X* = {:.5f}\n", target_x);

    print_derivative_results(x_span, y_span, target_x);

    return 0;
}