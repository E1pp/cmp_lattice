#pragma once

#include <string>
#include <vector>

namespace cmp_lattice::support {

///////////////////////////////////////////////////////////////////////////////

class FunctionFromFile {
 public:
  FunctionFromFile(double x_min, double x_max, size_t point_count, std::string_view path);

  double operator()(double x);

 private:
  const double x_min_;
  const double dx_;

  std::vector<double> vals_;

  static double GetStep(double x_min, double x_max, size_t point_count);

  void ReadFile(size_t point_count, std::string_view path);
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::support
