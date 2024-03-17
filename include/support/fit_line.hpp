#pragma once

#include <cstdlib>
#include <tuple>
#include <vector>

namespace cmp_lattice::support {

template <class T>
concept AlgebraOverDouble = std::regular<T> && requires (T a, T b, double pow, double scalar)
{
    { a + b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;

    { std::pow(a, pow) } -> std::same_as<T>;
    { scalar * a } -> std::same_as<T>;

    static_cast<T>(scalar);
};

// y = std::get<0>(tuple) * x + std::get<1>(tuple)
template <AlgebraOverDouble T>
std::tuple<T, T> FitLine(const std::vector<T>& x_data, const std::vector<T>& y_data);

} // namespace cmp_lattice::support

#define FIT_LINE_INL_H
#include "fit_line-inl.hpp"
#undef FIT_LINE_INL_H
