#pragma once

#ifndef FIT_LINE_INL_H
#error \
    "Direct inclusion of this file is not allowed. Include fit_line.hpp instead"
#include "fit_line.hpp"
#endif

#include <wheels/core/assert.hpp>

#include <numeric>

namespace cmp_lattice::support {

template <AlgebraOverDouble T>
std::tuple<T, T> FitLine(const std::vector<T>& x_data, const std::vector<T>& y_data)
{
    static constexpr T kZero = static_cast<T>(0);
    const int points_count = std::ssize(x_data);
    WHEELS_VERIFY(points_count == std::ssize(y_data), "Different sizes!");

    T x_tot = std::accumulate(x_data.begin(), x_data.end(), kZero);
    T y_tot = std::accumulate(y_data.begin(), y_data.end(), kZero);

    T xy_tot = std::inner_product(x_data.begin(), x_data.end(), y_data.begin(), kZero);
    T xx_tot = std::inner_product(x_data.begin(), x_data.end(), x_data.begin(), kZero);

    T k_denominator = points_count * xx_tot - std::pow(/*base=*/ x_tot, /*pow=*/ 2.0);
    WHEELS_VERIFY(k_denominator != 0, "Horizontal line detected");

    double k = (points_count * xy_tot - x_tot * y_tot) / k_denominator;

    double b = (y_tot - k * x_tot) / static_cast<double>(points_count);

    return std::make_pair(k, b);
}

} // namespace cmp_lattice::support
