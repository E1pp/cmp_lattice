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

/////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
concept Hamiltonian = requires (T ham,
                                std::vector<std::vector<int>>& ns_states,
                                std::vector<std::vector<int>>& r_states,
                                double scaling,
                                size_t i,
                                size_t j)
{
    T(ns_states, r_states, scaling);

    { T::IsDiagonal() } -> std::same_as<bool>;

    { ham.GetMatrixElement(i, j) } -> std::same_as<std::complex<double>>;
};

template <Hamiltonian HamiltonianPolicy>
arma::Mat<std::complex<double>> MakeMatrix(int momentum, int cutoff, double scaling_parameter, double perturbation_factor);

/////////////////////////////////////////////////////////////////////////////////////////////////

} // cmp_lattice::tffsa

#define MAKE_MATRIX_INL_H
#include "make_matrix-inl.hpp"
#undef MAKE_MATRIX_INL_H