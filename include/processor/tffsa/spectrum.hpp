#pragma once

#include "common.hpp"

#include <include/processor/eigen.hpp>

#include <filesystem>

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

struct GenerateSpectrumParameters {
    CachedFunctionsParameters cached;

    GridParameters sampling_grid;
    // Modified parameters: r.
    MatrixGenerationParameters matrix;
};

std::vector<arma::Col<std::complex<double>>>
GenerateSpectrum(
    GenerateSpectrumParameters params,
    std::filesystem::path path = std::filesystem::current_path());

///////////////////////////////////////////////////////////////////////////////

} // namespace cmp_lattice::tffsa
