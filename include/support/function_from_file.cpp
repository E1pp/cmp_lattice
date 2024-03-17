#include "function_from_file.hpp"

#include <wheels/core/assert.hpp>

#include <fstream>

namespace cmp_lattice::support {

///////////////////////////////////////////////////////////////////////////////

FunctionFromFile::FunctionFromFile(double x_min, double x_max, size_t point_count, std::string_view path)
    : x_min_(x_min),
      dx_(GetStep(x_min, x_max, point_count))
{
  ReadFile(point_count, path);
}

double FunctionFromFile::operator()(double x) {
  size_t pos = (x - x_min_) / dx_;

  WHEELS_VERIFY((pos >= 0 && pos <= vals_.size()), "Boundary check failed!");

  return vals_[pos];
}

double FunctionFromFile::GetStep(double x_min, double x_max, size_t point_count)
{
  if (point_count == 1) {
    WHEELS_VERIFY(x_min == x_max, "Different LB and RB with a single step!");
    return 1;
  }

  return (x_max - x_min) / static_cast<double>(point_count - 1);
}

void FunctionFromFile::ReadFile(size_t point_count, std::string_view path)
{
  vals_.reserve(point_count);

  std::ifstream stream(path);

  while (vals_.size() < point_count) {
    double value;
    stream >> value;
    vals_.push_back(value);
  }
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::support
