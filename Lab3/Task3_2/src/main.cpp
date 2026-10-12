#include "common.hpp"
#include "cubic_spline.hpp"

#include <matplot/matplot.h>
#include <matplot/backend/gnuplot.h>

#include <print>
#include <ranges>

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

void print_initial_nodes(
    std::span<const double> x,
    std::span<const double> f
) {
    std::println("Initail nodes:");
    print_span("x", x);
    print_span("f", f);
    std::println("");
}

void print_segment_table(std::span<const SplineSegment> segments)
{
    std::println("Table of coefficients and intervals");
    std::println("{:-^86}", "");
    std::cout << std::format("{:<3} | {:<12} | {:<10} | {:<10} | {:<10} | {:<10}\n", 
                             "i", "interval", "a_i", "b_i", "c_i", "d_i");
    std::println("{:-^86}", "");
    
    for (size_t i = 0; i < segments.size(); ++i)
    {
        const auto& seg = segments[i];
        std::println(
            "{:<3} | [{:<4.1f}, {:<4.1f}]  | {:<10.4f} | {:<10.4f} | {:<10.4f} | {:<10.4f}",
            i + 1, seg.x_left, seg.x_right, seg.a, seg.b, seg.c, seg.d
        );
    }
}

void print_spline_check(const CubicSpline& spline)
{
    std::println("Spline check:");
    std::println("{:-^52}", "");
    std::println("{:<5} | {:<10} | {:<10} | {:<12}", "i", "x_i", "f_i", "S(x_i)");
    std::println("{:-^52}", "");
    for (size_t i = 0; i < constants::x.size(); ++i)
    {
        double x_val = constants::x[i];
        double s_val = spline.evaluate(x_val);
        std::println(
            "{:<5} | {:<10.1f} | {:<10.4f} | {:<12.4f}", 
            i, x_val, constants::f[i], s_val
        );
    }
}

void print_second_derivative_check(const CubicSpline& spline)
{
    std::println("\nSecond derivative check:");
    std::println("S''(x_0) = {:.6f} (0.0 is expected)", spline.second_derivative_left());
    std::cout << std::format("S''(x_n) = {:.6f} (0.0 is expected)\n\n", spline.second_derivative_right());
}

void draw_plot(const CubicSpline& spline, const double eval_res)
{
    using namespace matplot;

    std::vector<double> x_dense;
    std::vector<double> y_dense;
    double x_min { constants::x.front() };
    double x_max { constants::x.back() };
    size_t resolution { 500 };
    double step { (x_max - x_min) / resolution };
    
    for (int i = 0; i <= resolution; ++i)
    {
        double current_x { x_min + i * step };
        x_dense.push_back(current_x);
        y_dense.push_back(spline.evaluate(current_x));
    }
    
    figure();
    matplot::gcf()->quiet_mode(true);

    auto be = std::dynamic_pointer_cast<matplot::backend::gnuplot>(matplot::gcf()->backend());
    if (be) {
        be->run_command("unset warnings\n");
    }

    hold(matplot::on);
    
    auto p_spline = plot(x_dense, y_dense, "-b");
    p_spline->line_width(2.0);
    p_spline->display_name("Cubic Spline");
    
    std::vector<double> x_nodes_vec(constants::x.begin(), constants::x.end());
    std::vector<double> y_values_vec(constants::f.begin(), constants::f.end());
    auto p_nodes = plot(x_nodes_vec, y_values_vec, "ro");
    p_nodes->marker_size(8)
            .marker_face_color("r")
            .display_name("Initial nodes");
    
    auto p_star = plot(std::vector{constants::x_star}, std::vector{eval_res}, "gp");
    p_star->marker_size(10)
           .marker_face_color("g")
           .display_name(std::format("x* ({:.2f}, {:.2f})", constants::x_star, eval_res));
    
    title("Cubic Spline Interpolation");
    xlabel("x");
    ylabel("f(x)");
    legend();
    grid(true);

    show();
}

int main()
{
    print_initial_nodes(constants::x, constants::f);

    auto spline_result = CubicSpline::build(constants::x, constants::f);
    if (!spline_result)
    {
        std::println("Error: {}", spline_result.error());
        return 1;
    }
    const auto& spline { *spline_result };
    const auto& segments { spline.get_segments() };

    print_segment_table(segments); 

    const auto eval_res { spline.evaluate(constants::x_star) };
    std::println("\nx* = {}", constants::x_star);
    std::println("S(x*) = {:.5f}\n", eval_res);

    print_spline_check(spline);

    print_second_derivative_check(spline);

    draw_plot(spline, eval_res);

    return 0;
}