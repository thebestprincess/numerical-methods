#include "common.hpp"
#include "newton_polynomial.hpp"
#include "lagrange_polynomial.hpp"

#include <matplot/matplot.h>

#include <cstddef>
#include <ranges>
#include <span>
#include <print>

void plot_results(
    std::span<const double> x_nodes, 
    std::span<const double> y_nodes,
    const LagrangePolynomial& lagrange, 
    const NewtonPolynomial& newton,
    double x_star
) {
    using namespace matplot;

    std::vector<double> x_dense, y_true, y_lag, y_newt;
    double x_start { x_nodes.front() - 0.2 };
    double x_end { x_nodes.back() + 0.2 };
    int steps { 200 };
    
    for (int i = 0; i <= steps; ++i)
    {
        double x = x_start + i * (x_end - x_start) / steps;
        x_dense.push_back(x);
        y_true.push_back(function(x));
        y_lag.push_back(lagrange(x));
        y_newt.push_back(newton(x));
    }

    figure();
    hold(on);

    auto p1 = plot(x_dense, y_true, "k-");
    p1->line_width(2).display_name("f(x) = tan(x)");

    auto p2 = plot(x_dense, y_lag, "r--");
    p2->line_width(2).display_name("Lagrange");

    auto p3 = plot(x_dense, y_newt, "b:");
    p3->line_width(2).display_name("Newton");

    std::vector<double> x_n(x_nodes.begin(), x_nodes.end());
    std::vector<double> y_n(y_nodes.begin(), y_nodes.end());
    auto p_nodes = plot(x_n, y_n, "mo");
    p_nodes->marker_face(true)
           .marker_size(8)
           .display_name("Nodes");

    auto p_star = plot(std::vector{x_star}, std::vector{function(x_star)}, "c*");
    p_star->marker_size(12)
          .line_width(2)
          .display_name("Point x*");

    title("Interpolation f(x) = tan(x)");
    xlabel("x");
    ylabel("y");
    legend();
    grid(on);

    show(); 
}

void print_span(
    std::string_view var_name,
    std::span<const double> v
) {
    std::print("{}: [", var_name);
    for (const auto [i, val] : std::views::enumerate(v))
    {
        std::print("{}", val);
        if (i != v.size() - 1) std::print(", ");
    }
    std::print("]\n");
}

void print_nodes(
    std::span<const double> x,
    std::span<const double> y
) {
    std::println("Interpolation nodes:");
    print_span("x", x);
    print_span("y", y);
    std::println("");
}

void print_lagrange_formula(
    std::span<const double> x,
    std::span<const double> y
) {
    std::println("Lagrange polynomial:");
    std::print("L(x) = ");
    for (size_t i = 0; i < x.size(); ++i)
    {
        if (i > 0) std::print(" + ");
        std::print("{:.4f}", y[i]);
        for (size_t j = 0; j < x.size(); ++j)
        {
            if (i != j) std::print(" * (x - {:.4f})/({:.4f} - {:.4f})", x[j], x[i], x[j]);
        }
    }
    std::println("\n");
}

void print_newton_formula(
    std::span<const double> x,
    std::span<const double> coefs
) {
    std::println("Newton polynomial:");
    std::print("N(x) = ");
    for (size_t i = 0; i < coefs.size(); ++i)
    {
        if (i > 0)
        {
            std::print("{}", (coefs[i] >= 0.0) ? " + " : " - ");
        }
        std::print("{:.5f}", std::abs(coefs[i]));
        for (size_t j = 0; j < i; ++j)
        {
            std::print("(x - {:.4f})", x[j]);
        }
    }
    std::println("\n");
}

void print_divided_differences_table(
    std::span<const double> x,
    std::span<const double> y
) {
    std::println("Table of divided differences:");
    const size_t n = x.size();
    std::vector<std::vector<double>> table(n, std::vector<double>(n, 0.0));
    
    for (size_t i = 0; i < n; ++i) table[i][0] = y[i];
    
    for (size_t j = 1; j < n; ++j)
    {
        for (size_t i = 0; i < n - j; ++i)
        {
            table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (x[i + j] - x[i]);
        }
    }
    
    for (size_t i = 0; i < n; ++i)
    {
        std::print("x={:.4f} | ", x[i]);
        for (size_t j = 0; j < n - i; ++j)
        {
            std::print("{:10.5f} ", table[i][j]);
        }
        std::println("");
    }
    std::println("");
}

int main(
    int argc,
    char** argv
) {
    std::span<const double> x_nodes = constants::set_of_points1;
    
    std::vector<double> y_nodes{};
    y_nodes.reserve(x_nodes.size());
    for (double x : x_nodes) y_nodes.push_back(function(x));

    print_nodes(x_nodes, y_nodes);

    LagrangePolynomial lagrange(x_nodes, y_nodes);
    NewtonPolynomial newton(x_nodes, y_nodes);

    print_lagrange_formula(x_nodes, y_nodes);

    print_divided_differences_table(x_nodes, y_nodes);
    print_newton_formula(x_nodes, newton.get_coefs());

    const double x_star { constants::x_star };
    const double exact_val { function(x_star) };
    const double lagrange_val { lagrange(x_star) };
    const double newton_val { newton(x_star) };

    std::println("x* = {:.5f}", x_star);
    std::println("Exact value of f(x*) = {:.8f}", exact_val);
    std::println("Value of the Lagrange polynomial = {:.8f}", lagrange_val);
    std::println("Value of the Newton polynomial = {:.8f}", newton_val);

    std::println("\nComputation errors:");
    std::println("Absolute error for Lagrange interpolation: {:.8e}", std::abs(exact_val - lagrange_val));
    std::println("Absolute error for Newton interpolation: {:.8e}", std::abs(exact_val - newton_val));
    std::println("Difference between Lagrange and Newton: {:.8e}", std::abs(lagrange_val - newton_val));

    // std::println("");
    // plot_results(x_nodes, y_nodes, lagrange, newton, x_star);

    return 0;
}