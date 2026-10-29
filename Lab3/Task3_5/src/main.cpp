#include "common.hpp"
#include "numerical_integration_strategies.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <print>
#include <string_view>

struct final_result
{
    std::string_view name;
    int p;
    double val_h1;
    double val_h2;
    double refined;
    double rr_error;
    double abs_error;
};

[[nodiscard]]
constexpr std::array<final_result, 3> generate_results() noexcept
{
    using namespace integration;
    using namespace constants;

    constexpr int n1 { static_cast<int>((xk - x0) / h1 + 0.5) };
    constexpr int n2 { static_cast<int>((xk - x0) / h2 + 0.5) };
    
    constexpr double exact_val { -6.0 / 49.0 };

    std::array<final_result, 3> results{};

    auto fill = [&](
        std::size_t idx,
        std::string_view name,
        int p,
        double v1,
        double v2
    ) constexpr noexcept
    {
        auto const rr { apply_runge_romberg(v1, v2, p) };
        results[idx] = final_result
        {
            name,
            p,
            v1,
            v2,
            rr.refined_value,
            rr.error_estimate,
            std::abs(exact_val - v2)
        };
    };

    fill(0, "Middle Rectangles", 2, middle_rectangles(func, x0, h1, n1), middle_rectangles(func, x0, h2, n2));
    fill(1, "Trapezoids", 2, trapezoids(func, x0, xk, h1, n1), trapezoids(func, x0, xk, h2, n2));
    fill(2, "Simpson", 4, simpson(func, x0, xk, h1, n1), simpson(func, x0, xk, h2, n2));

    return results;
}

void print_result(
    const auto& results,
    const double exact_val
) {
    std::println("{:-^116}", "");
    std::println("| {:<20} | {:>4} | {:>14} | {:>14} | {:>14} | {:>14} | {:>14} |",
               "Method", "p", "Integral (h1)", "Integral (h2)",
               "Refined (R-R)", "R-R Error", "Abs Error (h2)");
    std::println("{:-^116}", "");

    for (auto const& res : results)
    {
        std::println("| {:<20} | {:>4} | {:>14.6f} | {:>14.6f} | {:>14.6f} | {:>14.6f} | {:>14.6f} |",
                   res.name, res.p, res.val_h1, res.val_h2, res.refined, res.rr_error, res.abs_error);
    }
    std::println("{:-^116}\n", "");

    std::println("Exact analytic value of the integral: {:.6f}\n", exact_val);
}


int main()
{
    constexpr auto results { generate_results() };
    constexpr double exact_val { -6.0 / 49.0 };

    print_result(results, exact_val);

    const auto best_method { std::ranges::min_element(results, {}, &final_result::abs_error) };
    
    std::println("Method with the most accurate result (based on h2): {}", best_method->name);
    std::println("Its absolute error: {:.6f}", best_method->abs_error);

    return 0;
}