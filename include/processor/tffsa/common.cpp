#include "common.hpp"

#include <include/generator/tffsa/default_hamiltonian.hpp>

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

double MakeDelta(GridParameters grid) {
    return grid.points_count == 1
        ? 1
        : (grid.r_max - grid.r_min) / (grid.points_count - 1);
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

} // namespace cmp_lattice::tffsa
