#pragma once

#include "common.hpp"

#include <include/processor/eigen.hpp>

#include <filesystem>

namespace cmp_lattice::tffsa {

struct GenerateSpectrumParameters {
    CachedFunctionsParameters cached;
    MatrixSerialParameters matrix;
};

std::vector<arma::Col<std::complex<double>>>
GenerateSpectrum(
    GenerateSpectrumParameters params,
    std::filesystem::path path = std::filesystem::current_path());

} // namespace cmp_lattice::tffsa
