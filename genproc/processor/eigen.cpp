#include "eigen.hpp"

#include <wheels/core/assert.hpp>

namespace cmp_lattice {

///////////////////////////////////////////////////////////////////////////////

arma::Col<std::complex<double>> Eigenvalues(arma::Mat<std::complex<double>>& matrix) {  
  if (matrix.is_hermitian()) {
    arma::Col<double> eigs;
    arma::eig_sym(eigs, matrix);
    arma::Col<std::complex<double>> vector(eigs.size());

    for (size_t i = 0; i < eigs.size(); ++i) {
      vector[i] = eigs[i];
    }

    return vector;
  }
  arma::Col<std::complex<double>> vector;
  arma::eig_gen(vector, matrix);

  return vector;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice
