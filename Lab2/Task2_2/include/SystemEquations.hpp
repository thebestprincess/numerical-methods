#pragma once

#include <cstddef>
#include <array>
#include <cmath>

struct SystemEquations
{
    static constexpr double upper_bound { 3.0 };
    static constexpr double lower_bound { 2.8 };
    static constexpr double left_bound { 5.8 };
    static constexpr double right_bound { 6.0 };

    static constexpr size_t dimension { 2 }; 
    
    using XVec = std::array<double, dimension>;
    static constexpr XVec x0 { (left_bound + right_bound) / 2.0, (upper_bound + lower_bound) / 2.0  };
    
    
    static constexpr double f1 (double x1, double x2) { return (x1 * x1 + 16.0) * x2 - 64.0; }
    static constexpr double f1dx1 (double x1, double x2) { return 2.0 * x2 * x1; }
    static constexpr double f1dx2 (double x1, double x2) { return x1 * x1 + 16.0; }

    static constexpr double f1(const XVec& x_vec) { return f1(x_vec[0], x_vec[1]); }    
    static constexpr double f1dx1 (const XVec& x_vec) { return f1dx1(x_vec[0], x_vec[1]); }
    static constexpr double f1dx2 (const XVec& x_vec) { return f1dx2(x_vec[0], x_vec[1]); }
    
    static constexpr double f2 (double x1, double x2) { return (x1 - 2.0) * (x1 - 2.0) + (x2 - 2.0) * (x2 - 2.0) - 16.0; }
    static constexpr double f2dx1 (double x1, double x2) { return 2.0 * x1 - 4.0; }
    static constexpr double f2dx2 (double x1, double x2) { return 2.0 * x2 - 4.0; }

    static constexpr double f2 (const XVec& x_vec) { return f2(x_vec[0], x_vec[1]); }
    static constexpr double f2dx1 (const XVec& x_vec) { return f2dx1(x_vec[0], x_vec[1]); }
    static constexpr double f2dx2 (const XVec& x_vec) { return f2dx2(x_vec[0], x_vec[1]); }

    static constexpr double phi1(double x1, double x2) { return 2.0 + std::sqrt(16.0 - (x2 - 2.0) * (x2 - 2.0)); }
    static constexpr double phi2(double x1, double x2) { return 64.0 / (x1 * x1 + 16.0); }

    static constexpr double phi1dx1(double x1, double x2) { return -128.0 * x1 / ((x1 * x1 + 16.0) * (x1 * x1 + 16.0)); }
    static constexpr double phi1dx2(double x1, double x2) { return 0.0; }
    static constexpr double phi2dx1(double x1, double x2) { return 0.0; }
    static constexpr double phi2dx2(double x1, double x2) { return -(x2 - 2.0) / std::sqrt(16.0 - (x2 - 2.0) * (x2 - 2.0)); }

    static constexpr double phi1(const XVec& x) { return phi1(x[0], x[1]); }
    static constexpr double phi2(const XVec& x) { return phi2(x[0], x[1]); }

    static constexpr double phi1dx1(const XVec& x) { return phi1dx1(x[0], x[1]); }
    static constexpr double phi1dx2(const XVec& x) { return phi1dx2(x[0], x[1]); }
    static constexpr double phi2dx1(const XVec& x) { return phi2dx1(x[0], x[1]); }
    static constexpr double phi2dx2(const XVec& x) { return phi2dx2(x[0], x[1]); }
};