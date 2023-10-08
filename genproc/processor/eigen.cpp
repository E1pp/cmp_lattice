#include "eigen.hpp"

#include <wheels/core/assert.hpp>

namespace cmp_lattice {

///////////////////////////////////////////////////////////////////////////////

arma::Col<double> Eigenvalues(arma::Mat<std::complex<double>>& matrix) {
  arma::Col<double> vector;
  auto status = arma::eig_sym(vector, matrix);

  WHEELS_VERIFY(status, "Diagonalization failed!");

  return vector;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice
