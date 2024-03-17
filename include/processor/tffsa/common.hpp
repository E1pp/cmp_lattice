#pragma once

#include <complex>
#include <filesystem>

namespace cmp_lattice::tffsa {

struct GridParameters {
    double r_min;
    double r_max;
    size_t points_count;
};

struct CachedFunctionsParameters {
    GridParameters cached_grid;
    int lambda;
    std::filesystem::path common_path;
};

struct MatrixGenerationParameters {
    int momentum;
    int lambda;
    double scaling;
    std::complex<double> zeta;
    double mass;
};

struct MatrixSerialParameters {
    MatrixGenerationParameters def;
    GridParameters grid;
};

} // namespace cmp_lattice::tffsa