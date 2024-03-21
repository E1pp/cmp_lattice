#pragma once

#include <armadillo>

#include <complex>
#include <filesystem>

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

struct GridParameters {
    double r_min;
    double r_max;
    size_t points_count;
};

double MakeDelta(GridParameters grid);

///////////////////////////////////////////////////////////////////////////////

struct CachedFunctionsParameters {
    GridParameters cached_grid;
    int lambda;
    std::filesystem::path common_path;
};

void PrepareCachedFunctions(CachedFunctionsParameters params);

///////////////////////////////////////////////////////////////////////////////

struct MatrixGenerationParameters {
    int momentum;
    int lambda;
    double scaling;
    std::complex<double> zeta;
    double mass;
};

arma::Mat<std::complex<double>>
GenerateMatrix(MatrixGenerationParameters params);

///////////////////////////////////////////////////////////////////////////////

} // namespace cmp_lattice::tffsa