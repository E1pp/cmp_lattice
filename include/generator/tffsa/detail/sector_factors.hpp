#pragma once

#include <include/generator/tffsa/sectors.hpp>

#include <string_view>
#include <vector>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

void StartSeries(double r_min, double r_max, size_t r_n, size_t lambda,
                 std::string_view path);

///////////////////////////////////////////////////////////////////////////////

double StateFactor(double scaling_factor, Sector sector,
                   const std::vector<int>& state);

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail
