#pragma once

#include "sectors.hpp"

#include <cmath>
#include <vector>

namespace cmp_lattice::tffsa {

double TanFactor(const std::vector<int>& state, Sector sector, double scale_parameter);

double CotFactor(NSState ns_state, RState r_state, double scale_parameter);

} // namespace cmp_lattice::tffsa