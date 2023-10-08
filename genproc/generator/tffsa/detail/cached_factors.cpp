#include "cached_factors.hpp"

#include <wheels/core/assert.hpp>

#include <fstream>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

KFunction::KFunction(double r_min, double r_max, size_t r_steps, size_t lambda,
                     std::string_view path)
    : r_min_(r_min),
      dr_([=]() -> double {
        if (r_steps == 1) {
          WHEELS_VERIFY(r_min == r_max, "Can't have infinite lin steps!");

          return 1;
        }

        return (r_max - r_min) / static_cast<double>(r_steps - 1);
      }()),
      lambda_steps_(2 * lambda + 1) {
  std::ifstream stream(path);

  vals_.reserve(r_steps);

  for (size_t i = 0; i < r_steps; ++i) {
    std::vector<double> row(lambda_steps_);

    for (auto& val : row) {
      stream >> val;
    }

    vals_.push_back(std::move(row));
  }
}

///////////////////////////////////////////////////////////////////////////////

double KFunction::operator()(double r, double i) {
  size_t r_idx = (r - r_min_) / dr_;

  WHEELS_VERIFY((r_idx >= 0 && r_idx < vals_.size()),
                "Boundary check for r failed!");

  i = std::abs(i);

  size_t vec_idx = 2 * i;

  WHEELS_VERIFY((vec_idx >= 0 && vec_idx < lambda_steps_),
                "Boundary check for vector comp failed!");

  return vals_[r_idx][vec_idx];
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail