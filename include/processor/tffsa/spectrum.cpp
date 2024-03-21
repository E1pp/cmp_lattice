#include "spectrum.hpp"

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

std::vector<arma::Col<std::complex<double>>>
GenerateSpectrum(
    GenerateSpectrumParameters params,
    std::filesystem::path path)
{
    params.cached.common_path = path;
    
    PrepareCachedFunctions(params.cached);

    std::vector<arma::Col<std::complex<double>>> ret{};
    const auto& grid = params.sampling_grid;
    ret.reserve(grid.points_count);

    double delta = MakeDelta(grid);
    for (size_t iter = 0; iter < grid.points_count; ++iter) {
        MatrixGenerationParameters mat = params.matrix;
        mat.scaling = grid.r_min + iter * delta;

        auto matrix = GenerateMatrix(mat);
        ret.push_back(Eigenvalues(matrix));
    }

    return ret;
};

///////////////////////////////////////////////////////////////////////////////

} // namespace cmp_lattice::tffsa