#pragma once

#include "sectors.hpp"

#include <complex>

namespace cmp_lattice::tffsa {

std::complex<double> FormFactor(NSState ns_state, RState r_state, double scaling_factor);

} // namespace cmp_lattice::tffsa
