#pragma once

#include "tridiagonal_solver.hpp"

#include <expected>
#include <string_view>
#include <span>
#include <vector>

struct SplineSegment
{
    double x_left;
    double x_right;
    double a;
    double b;
    double c;
    double d;
};

class CubicSpline final
{
private:
    std::vector<SplineSegment> segments_;

    explicit CubicSpline(std::vector<SplineSegment> segments);

public:
    [[nodiscard]]
    static auto build(
        std::span<const double> x, 
        std::span<const double> f
    ) -> std::expected<CubicSpline, std::string_view>;

    [[nodiscard]]
    double evaluate(double x_val) const;

    [[nodiscard]]
    double second_derivative_left() const;

    [[nodiscard]]
    double second_derivative_right() const;

    [[nodiscard]]
    auto get_segments() const -> const std::vector<SplineSegment>&;

private:
    [[nodiscard]]
    static TriSystem obtain_trisystem(
        std::span<const double> h,
        std::span<const double> f,
        const size_t matrix_size 
    ) noexcept;
};