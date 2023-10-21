#include "trig_factors.hpp"

#include <numbers>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

double Tan(double x, double y) {
  return (y - x) /
         (std::sqrt(1 + std::pow(/*base=*/ x, /*pow=*/ 2.0)) + std::sqrt(1 + std::pow(/*base=*/ y,/*pow=*/ 2.0)));
}

///////////////////////////////////////////////////////////////////////////////

double Cot(double x, double y) {
  return (std::sqrt(1 + std::pow(/*base=*/ x,/*pow=*/ 2.0)) + std::sqrt(1 + std::pow(/*base=*/ y,/*pow=*/ 2.0))) /
         (x - y);
}

///////////////////////////////////////////////////////////////////////////////

double TanFactor(const std::vector<int>& state, Sector sector,
                 double scale_parameter) {
  if (state.size() < 2) {
    return 1.0;
  }

  double sector_multiplier = sector == Sector::R ? 1.0 : 0.5;

  double product = 1.0;

  for (size_t i = 0; i < state.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      product *= Tan(2 * std::numbers::pi * state[i] * sector_multiplier / scale_parameter,
                     2 * std::numbers::pi * state[j] * sector_multiplier / scale_parameter);
    }
  }

  return product;
}

///////////////////////////////////////////////////////////////////////////////

double CotFactor(NSState ns_state, RState r_state, double scale_parameter) {
  std::vector state1 = std::move(ns_state.impl);

  std::vector state2 = std::move(r_state.impl);

  if (state1.empty() || state2.empty()) {
    return 1.0;
  }

  double product = 1.0;

  for (auto num1 : state1) {
    for (auto num2 : state2) {
      product *= Cot(2 * std::numbers::pi * num1 * 0.5 / scale_parameter,
                     2 * std::numbers::pi * num2 / scale_parameter);
    }
  }

  return product;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail
