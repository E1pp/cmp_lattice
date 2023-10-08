#include "form_factor.hpp"

#include "cached_factors.hpp"
#include "trig_factors.hpp"

#include <cmath>
#include <numbers>
#include <optional>

namespace cmp_lattice::tffsa::detail {

using namespace std::complex_literals;

///////////////////////////////////////////////////////////////////////////////

static constexpr double kPrefactor =
    1.35784;  // 2^(1/12) * e^(-1/8) Glaisher^(3/2)

///////////////////////////////////////////////////////////////////////////////

std::complex<double> ImaginaryFactor(std::size_t a, std::size_t b) {
  std::size_t half_sum = (a + b) >> 1;

  return std::exp(0.5i * std::numbers::pi * static_cast<double>(half_sum));
}

///////////////////////////////////////////////////////////////////////////////

std::complex<double> FormFactor(NSState ns_state, RState r_state,
                                double scaling_factor) {
  auto& ns_vec = ns_state.impl;
  auto& r_vec = r_state.impl;

  auto result = ImaginaryFactor(ns_vec.size(), r_vec.size()) * kPrefactor;

  result *= TanFactor(ns_vec, Sector::NS, scaling_factor);
  result *= TanFactor(r_vec, Sector::R, scaling_factor);
  result *= CotFactor(ns_state, r_state, scaling_factor);

  return result;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail