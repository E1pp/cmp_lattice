#pragma once

#include <genproc/generator/tffsa/sectors.hpp>

#include <complex>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

std::complex<double> FormFactor(NSState ns_state, RState r_state,
                                double scaling_factor);

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail
