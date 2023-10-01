#pragma once

#include "sectors.hpp"

#include <armadillo>

#include <complex>

namespace cmp_lattice::tffsa {

void StartSeries(double r_min,
    double r_max,
    size_t r_n,
    size_t lambda,
    std::string common_path);

arma::Mat<std::complex<double>> MakeMatrix(int momentum, int cutoff, double scaling_parameter);

} // cmp_lattice::tffsa