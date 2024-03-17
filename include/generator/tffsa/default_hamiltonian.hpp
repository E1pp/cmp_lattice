#pragma once

#include <include/generator/tffsa/make_matrix.hpp>

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

class DefaultHamiltonian {
 public:
  DefaultHamiltonian(std::vector<std::vector<int>>& ns_states,
                     std::vector<std::vector<int>>& r_states, double scaling);

  std::complex<double> GetMatrixElement(size_t i, size_t j);

  static constexpr bool IsDiagonal() {
    return true;
  }

  static void StartSeries(double r_min, double r_max, size_t r_n, size_t lambda,
                          std::string path);

 private:
  std::vector<std::vector<int>>& ns_states_;
  std::vector<std::vector<int>>& r_states_;
  const double scaling_;
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa
