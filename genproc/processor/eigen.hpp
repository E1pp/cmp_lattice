#pragma once

#define ARMA_USE_LAPACK

#include <armadillo>

#include <complex>

namespace cmp_lattice {

arma::Col<double> Eigenvalues(arma::Mat<std::complex<double>>& matrix);

} // namespace cmp_lattice
