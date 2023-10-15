#include "sector_factors.hpp"

#include "cached_factors.hpp"

#include <wheels/core/assert.hpp>

#include <numbers>
#include <optional>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

static std::optional<KFunction> k_function;

///////////////////////////////////////////////////////////////////////////////

void StartSeries(double r_min, double r_max, size_t r_n, size_t lambda,
                 std::string_view path) {
  if (k_function) {
    return;
  }

  k_function.emplace(r_min, r_max, r_n, lambda, path);
}

///////////////////////////////////////////////////////////////////////////////

double MakePrefactor(double scale_parameter, double component, Sector sector) {
  WHEELS_ASSERT(k_function.has_value(), "Must start series first!");

  double sector_prefactor = sector == Sector::R ? 1.0 : -1.0;

  double exponent =
      sector_prefactor * ((*k_function)(scale_parameter, component));

  double denominator = std::pow(
    /*base=*/ std::pow(scale_parameter, 2) + std::pow(2 * std::numbers::pi * component, 2),
    /*exp=*/ 0.25);

  return std::exp(exponent) / denominator;
}

///////////////////////////////////////////////////////////////////////////////

double StateFactor(double scale_parameter, Sector sector,
                   const std::vector<int>& state) {
  double sector_multiplier = sector == Sector::R ? 1.0 : 0.5;

  double result = 1.0;

  for (auto num : state) {
    result *= MakePrefactor(scale_parameter, num * sector_multiplier, sector);
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail