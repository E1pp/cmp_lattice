#include "default_hamiltonian.hpp"
#include "cached_factors.hpp"

#include <wheels/core/assert.hpp>

#include <optional>

namespace cmp_lattice::tffsa {

/////////////////////////////////////////////////////////////////////////////////////////////////

static std::optional<FunctionFromFile> cached_h0ns;
static std::optional<FunctionFromFile> cached_h0r;

/////////////////////////////////////////////////////////////////////////////////////////////////

double GetSectorOffset(Sector sector, double scaling)
{
    WHEELS_VERIFY(cached_h0ns && cached_h0r, "You must start series first!");

    auto& func = sector == Sector::NS ? *cached_h0ns : *cached_h0r;

    return func(scaling);
}

std::complex<double> GetMatrixElement(std::vector<int>& state, Sector sector, double scaling)
{
    double sector_multiplier = sector == Sector::NS ? 0.5 : 1;

    std::complex<double> ham = GetSectorOffset(sector, scaling);

    for (auto elem : state) {
        double expr = elem * sector_multiplier / scaling;

        ham += std::sqrt(1 + std::pow(/*base=*/ expr, /*exp=*/ 2));
    }

    return ham;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

DefaultHamiltonian::DefaultHamiltonian(
    std::vector<std::vector<int>>& ns_states,
    std::vector<std::vector<int>>& r_states,
    double scaling)
    : ns_states_(ns_states)
    , r_states_(r_states)
    , scaling_(scaling)
{ }

std::complex<double> DefaultHamiltonian::GetMatrixElement(size_t i, size_t j)
{
    WHEELS_VERIFY(i == j, "We are diagonal matrix!");

    if (i < ns_states_.size()) {
        return tffsa::GetMatrixElement(ns_states_[i], Sector::NS, scaling_);
    } else {
        WHEELS_VERIFY(i - ns_states_.size() < r_states_.size(), "Index is too big!");

        return tffsa::GetMatrixElement(r_states_[i - ns_states_.size()], Sector::R, scaling_);
    }
}

void DefaultHamiltonian::StartSeries(double r_min,
    double r_max,
    size_t r_n,
    size_t /*lambda*/,
    std::string path)
{
    if (cached_h0ns && cached_h0r) {
        return;
    }

    std::string h0ns_path = path + "/H0NS_vals.txt";
    std::string h0r_path = path + "/H0R_vals.txt";

    cached_h0ns.emplace(r_min, r_max, r_n, h0ns_path);
    cached_h0r.emplace(r_min, r_max, r_n, h0r_path);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace cmp_lattice::tffsa
