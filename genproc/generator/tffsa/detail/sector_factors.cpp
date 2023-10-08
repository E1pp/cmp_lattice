#include "sector_factors.hpp"

#include "cached_factors.hpp"

#include <wheels/core/assert.hpp>

#include <numbers>
#include <optional>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

static std::optional<KFunction> cached_functions;

///////////////////////////////////////////////////////////////////////////////

void StartSeries(double r_min, double r_max, size_t r_n, size_t lambda,
                 std::string_view path) {
  if (cached_functions) {
    return;
  }

  cached_functions.emplace(r_min, r_max, r_n, lambda, path);
}

///////////////////////////////////////////////////////////////////////////////

double MakePrefactor(double scale_parameter, double component, Sector sector) {
  WHEELS_ASSERT(cached_functions.has_value(), "Must start series first!");

  double sector_prefactor = sector == Sector::R ? 1.0 : -1.0;

  double exponent =
      sector_prefactor * ((*cached_functions)(scale_parameter, component));

  double denominator = std::sqrt(
      2 * std::numbers::pi *
      std::sqrt(component * component + scale_parameter * scale_parameter));

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