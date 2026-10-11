#include "cubic_spline.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <format>

namespace
{

TriSystem obtain_trisystem(
    std::span<const double> h,
    std::span<const double> f,
    const size_t matrix_size 
) noexcept
{
    TriSystem sys;
    sys.a.resize(matrix_size, 0.0);
    sys.b.resize(matrix_size, 0.0);
    sys.c.resize(matrix_size, 0.0);
    sys.d.resize(matrix_size, 0.0);

    for (size_t k : std::views::iota(0uz, matrix_size))
    {
        size_t i { k + 2 };
        
        sys.d[k] = 3.0 * ((f[i] - f[i - 1]) / h[i] - (f[i - 1] - f[i - 2]) / h[i - 1]);
        sys.b[k] = 2.0 * (h[i - 1] + h[i]);

        if (k > 0)
        {
            sys.a[k] = h[i - 1];
        }

        if (k < matrix_size - 1)
        {
            sys.c[k] = h[i];
        }
    }

    return sys;
}

std::vector<double> obtain_cs(
    std::span<const double> h,
    std::span<const double> f,
    size_t interval_count
) 
{
    const size_t matrix_size { interval_count - 1 };
    TriSystem sys { obtain_trisystem(h, f, matrix_size) };
    auto solve_result { TriSolver::solve(sys) };
    if (!solve_result) throw std::invalid_argument(std::format("TriSolver error: {}", solve_result.error()));
    const auto& c_solution = solve_result->x;

    std::vector<double> c_full(interval_count + 1, 0.0);
    for (size_t k = 0; k < matrix_size; ++k) {
        c_full[k + 2] = c_solution[k];
    }

    return c_full;
}

std::vector<double> obtain_grid_steps(
    std::span<const double> x,
    size_t interval_count
) noexcept
{
    std::vector<double> grid_steps(interval_count + 1);
    for (size_t i : std::views::iota(1uz, interval_count + 1))
    {
        grid_steps[i] = x[i] - x[i - 1];
    }

    return grid_steps;
}

std::vector<SplineSegment> evaluate_segments(
    std::span<const double> x,
    std::span<const double> f,
    std::span<const double> c_full,
    std::span<const double> grid_steps,
    size_t interval_count
)
{
    std::vector<SplineSegment> segments;
    segments.reserve(interval_count);
    for (size_t i : std::views::iota(1uz, interval_count + 1))
    {
        double a_val { f[i - 1] };
        double b_val { 0.0 }; 
        double d_val { 0.0 };

        if (i < interval_count)
        {
            b_val = (f[i] - f[i - 1]) / grid_steps[i] - (1.0 / 3.0) * grid_steps[i] * (c_full[i + 1] + 2.0 * c_full[i]);
            d_val = (c_full[i + 1] - c_full[i]) / (3.0 * grid_steps[i]);
        }
        else
        {
            b_val = (f[i] - f[i - 1]) / grid_steps[i] - (2.0 / 3.0) * grid_steps[i] * c_full[i];
            d_val = -c_full[i] / (3.0 * grid_steps[i]);
        }

        segments.push_back(
            SplineSegment
            {
                .x_left = x[i - 1],
                .x_right = x[i],
                .a = a_val,
                .b = b_val,
                .c = c_full[i],
                .d = d_val
            }
        );
    }

    return segments;
}
    
}

CubicSpline::CubicSpline(std::vector<SplineSegment> segments)
        : segments_(std::move(segments)) {}

auto CubicSpline::build(
    std::span<const double> x, 
    std::span<const double> f
) -> std::expected<CubicSpline, std::string_view>
{
    if (x.size() != f.size()) throw std::invalid_argument("x.size() != f.size()");

    const size_t interval_count { x.size() - 1 };
    if (interval_count < 3) throw std::invalid_argument("x.size() must be >= 3");
    
    std::vector<double> grid_steps { obtain_grid_steps(x, interval_count) };
    std::vector<double> c_full { obtain_cs(grid_steps, f, interval_count) };

    auto segments { evaluate_segments(x, f, c_full, grid_steps, interval_count) };

    return CubicSpline{std::move(segments)};
}

double CubicSpline::evaluate(double x_val) const
{
    auto it = std::upper_bound(
        segments_.begin(),
        segments_.end(),
        x_val, 
        [](double val, const SplineSegment& seg)
        {
            return val < seg.x_left;
        }
    );

    if (it == segments_.begin())
    {
        return segments_.front().a;
    }
        
    const auto& seg { *(it - 1) };
    double dx { x_val - seg.x_left };
    
    return seg.a + dx * (seg.b + dx * (seg.c + dx * seg.d));
}


double CubicSpline::second_derivative_left() const
{
    return 2.0 * segments_.front().c;
}

double CubicSpline::second_derivative_right() const
{
    const auto& last = segments_.back();
    double h_n = last.x_right - last.x_left;

    return 2.0 * last.c + 6.0 * last.d * h_n;
}

auto CubicSpline::get_segments() const -> const std::vector<SplineSegment>&
{
    return segments_;
}