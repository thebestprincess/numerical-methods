#include "common.hpp"
#include "error_metrics.hpp"
#include "polynomial_evaluation.hpp"

#include <matplot/matplot.h>

#include <span>
#include <print>
#include <ranges>

template <std::size_t Degree>
void print_polynomial_info(
    std::span<const double, Degree + 1> coeffs,
    ErrorMetrics err
) noexcept
{
    std::println("Polynomial Degree {}", Degree);
    std::print("Coefficients: ");
    for (double c : coeffs)
    {
        std::print("{:.5f} ", c);
    }
    std::println("");
    
    std::print("Explicit form: P_{}(x) = {:.5f}", Degree, coeffs[0]);
    for (std::size_t i = 1; i <= Degree; ++i)
    {
        if (coeffs[i] >= 0) std::print(" + ");
        else std::print(" - ");
        
        std::print("{:.5f}", std::abs(coeffs[i]));
        if (i == 1) std::print(" * x");
        else std::print(" * x^{}", i);
    }

    std::println("\nSSE (Sum of Squared Errors): {:.5f}", err.sse);
    std::println("MSE (Mean Squared Error): {:.5f}\n", err.mse);
}

constexpr double eval_poly(
    double x,
    std::span<const double> coeffs
) noexcept
{
    double y { 0.0 };
    double x_pow { 1.0 };
    for (double c : coeffs)
    {
        y += c * x_pow;
        x_pow *= x;
    }
    
    return y;
}

void display_plot(
    std::span<const double> coeffs1,
    std::span<const double> coeffs2,
    std::span<const double> coeffs3
) {
    const std::vector<double> x_pts(constants::x_data.begin(), constants::x_data.end());
    const std::vector<double> y_pts(constants::y_data.begin(), constants::y_data.end());

    const std::vector<double> x_dense = matplot::linspace(-1.5, 4.0, 400);

    const auto y_poly1 = x_dense 
        | std::views::transform([coeffs1](double x) { return eval_poly(x, coeffs1); }) 
        | std::ranges::to<std::vector>();
        
    const auto y_poly2 = x_dense 
        | std::views::transform([coeffs2](double x) { return eval_poly(x, coeffs2); }) 
        | std::ranges::to<std::vector>();
        
    const auto y_poly3 = x_dense 
        | std::views::transform([coeffs3](double x) { return eval_poly(x, coeffs3); }) 
        | std::ranges::to<std::vector>();

    matplot::figure();
    matplot::gcf()->quiet_mode(true);

    auto be = std::dynamic_pointer_cast<matplot::backend::gnuplot>(matplot::gcf()->backend());
    if (be) be->run_command("unset warnings\n");

    matplot::hold(matplot::on);

    matplot::plot(x_pts, y_pts, "ro")->marker_size(8).marker_face_color("r");
    matplot::plot(x_dense, y_poly1, "b-")->line_width(2);
    matplot::plot(x_dense, y_poly2, "g--")->line_width(2);
    matplot::plot(x_dense, y_poly3, "m-.")->line_width(2);

    matplot::title("Least Squares Method - Polynomial Approximation");
    matplot::xlabel("x");
    matplot::ylabel("y");
    
    matplot::legend({
        "Original Data", 
        "Polynomial Degree 1", 
        "Polynomial Degree 2", 
        "Polynomial Degree 3"
    });
    
    matplot::grid(matplot::on);
    matplot::show();
}


int main()
{
    constexpr auto coeffs1_result { build_polynomial<1, 6>(constants::x_data, constants::y_data) };
    constexpr auto coeffs2_result { build_polynomial<2, 6>(constants::x_data, constants::y_data) };
    constexpr auto coeffs3_result { build_polynomial<3, 6>(constants::x_data, constants::y_data) };

    static_assert(
        coeffs1_result.has_value()
        && coeffs2_result.has_value()
        && coeffs3_result.has_value(), 
        "Mathematical failure during compile-time matrix inversion."
    );

    constexpr auto coeffs1 { *coeffs1_result };
    constexpr auto coeffs2 { *coeffs2_result };
    constexpr auto coeffs3 { *coeffs3_result };

    constexpr auto err1 { compute_errors<1, 6>(constants::x_data, constants::y_data, coeffs1) };
    constexpr auto err2 { compute_errors<2, 6>(constants::x_data, constants::y_data, coeffs2) };
    constexpr auto err3 { compute_errors<3, 6>(constants::x_data, constants::y_data, coeffs3) };

    std::println("Least Squares Method Polynomial Fitter\n");
    
    print_polynomial_info<1>(coeffs1, err1);
    print_polynomial_info<2>(coeffs2, err2);
    print_polynomial_info<3>(coeffs3, err3);
    
    std::println("Error Comparison");
    std::println("MSE1: {:.6f}", err1.mse);
    std::println("MSE2: {:.6f}", err2.mse);
    std::println("MSE3: {:.6f}", err3.mse);
    
    display_plot(coeffs1, coeffs2, coeffs3);
    return 0;
}
