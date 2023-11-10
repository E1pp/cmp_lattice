#pragma once

#include <cstdlib>
#include <tuple>
#include <vector>

namespace cmp_lattice::support {

// y = std::get<0>(tuple) * x + std::get<1>(tuple)
std::tuple<double, double> FitLine(const std::vector<double>& x_data, const std::vector<double> y_data);

} // namespace cmp_lattice::support
