#pragma once

#include <include/support/function_from_file.hpp>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

using support::FunctionFromFile;

class KFunction {
 public:
  KFunction(double r_min, double r_max, size_t r_n, size_t lambda,
            std::string_view path);

  double operator()(double r, double i);

 private:
  const double r_min_;
  const double dr_;

  const size_t lambda_steps_;

  std::vector<std::vector<double>> vals_;
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail
