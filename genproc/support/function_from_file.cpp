#include "function_from_file.hpp"

#include <wheels/core/assert.hpp>

#include <fstream>

namespace cmp_lattice::support {

///////////////////////////////////////////////////////////////////////////////

FunctionFromFile::FunctionFromFile(double r_min, double r_max, size_t steps,
                                   std::string_view path)
    : r_min_(r_min),
      dr_([=]() -> double {
        if (steps == 1) {
          WHEELS_VERIFY(r_min == r_max, "Can't have infinite lin steps!");

          return 1;
        }

        return (r_max - r_min) / static_cast<double>(steps - 1);
      }()) {
  vals_.reserve(steps);

  std::ifstream stream(path);

  for (size_t i = 0; i < steps; ++i) {
    double var = 0;

    stream >> var;

    vals_.push_back(var);
  }
}

///////////////////////////////////////////////////////////////////////////////

double FunctionFromFile::operator()(double r) {
  size_t pos = (r - r_min_) / dr_;

  WHEELS_VERIFY((pos >= 0 && pos <= vals_.size()), "Boundary check failed!");

  return vals_[pos];
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::support
