#include "spectrum.hpp"

#include <include/generator/tffsa/default_hamiltonian.hpp>

namespace cmp_lattice::tffsa {

void PrepareCachedFunctions(CachedFunctionsParameters params)
{
    tffsa::DefaultHamiltonian::StartSeries(
        params.cached_grid.r_min,
        params.cached_grid.r_max,
        params.cached_grid.points_count,
        params.lambda,
        params.common_path);
    tffsa::StartSeries(
        params.cached_grid.r_min,
        params.cached_grid.r_max,
        params.cached_grid.points_count,
        params.lambda,
        params.common_path);
}

arma::Mat<std::complex<double>>
GenerateMatrix(MatrixGenerationParameters params)
{
    return tffsa::MakeMatrix<DefaultHamiltonian>(
        params.momentum,
        params.lambda,
        params.scaling,
        params.zeta,
        params.mass);
}

double MakeDelta(GridParameters grid) {
    return grid.points_count == 1
        ? 1
        : (grid.r_max - grid.r_min) / (grid.points_count - 1);
}

std::vector<arma::Col<std::complex<double>>>
GenerateSpectrum(
    GenerateSpectrumParameters params,
    std::filesystem::path path)
{
    params.cached.common_path = path;
    
    PrepareCachedFunctions(params.cached);

    std::vector<arma::Col<std::complex<double>>> ret{};
    const auto& grid = params.matrix.grid;
    ret.reserve(grid.points_count);

    double delta = MakeDelta(grid);

    for (size_t iter = 0; iter < grid.points_count; ++iter) {
        MatrixGenerationParameters mat = params.matrix.def;
        mat.scaling = grid.r_min + iter * delta;

        auto matrix = GenerateMatrix(mat);
        ret.push_back(Eigenvalues(matrix));
    }

    return ret;
};

} // namespace cmp_lattice::tffsa