#pragma once

#include <concepts>
#include <cstddef>

namespace concepts
{

template <typename System>
concept SysEq = requires (const System& s, double x1, double x2)
{
    typename System::XVec;

    { System::dimension } -> std::convertible_to<size_t>;

    { System::f1(x1, x2) } -> std::convertible_to<double>;
    { System::f2(x1, x2) } -> std::convertible_to<double>;

    { System::f1dx1(x1, x2) } -> std::convertible_to<double>;
    { System::f1dx2(x1, x2) } -> std::convertible_to<double>;
    { System::f2dx1(x1, x2) } -> std::convertible_to<double>;
    { System::f2dx2(x1, x2) } -> std::convertible_to<double>;
};

template <typename System>
concept SimpleIterSysEq = requires(const System& s, double x1, double x2)
{
    typename System::XVec;
    { System::dimension } -> std::convertible_to<std::size_t>;

    { System::phi1(x1, x2) } -> std::convertible_to<double>;
    { System::phi2(x1, x2) } -> std::convertible_to<double>;

    { System::phi1dx1(x1, x2) } -> std::convertible_to<double>;
    { System::phi1dx2(x1, x2) } -> std::convertible_to<double>;
    { System::phi2dx1(x1, x2) } -> std::convertible_to<double>;
    { System::phi2dx2(x1, x2) } -> std::convertible_to<double>;
};

} // namespace concepts