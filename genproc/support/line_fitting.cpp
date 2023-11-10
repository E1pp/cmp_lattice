#include "line_fitting.hpp"

#include <wheels/core/assert.hpp>

#include <numeric>
#include <ranges>

namespace cmp_lattice::support {

std::tuple<double, double> FitLine(const std::vector<double>& x_data, const std::vector<double> y_data)
{
    const int points_count = std::ssize(x_data);

    WHEELS_VERIFY(points_count == std::ssize(y_data), "Different sizes!");

    double x_tot = std::accumulate(x_data.begin(), x_data.end(), 0);
    double y_tot = std::accumulate(y_data.begin(), y_data.end(), 0);

    double xy_tot = std::inner_product(x_data.begin(), x_data.end(), y_data.begin(), 0);
    double xx_tot = std::inner_product(x_data.begin(), x_data.end(), x_data.begin(), 0);

    double k_denominator = points_count * xx_tot - std::pow(/*base=*/ x_tot, /*pow=*/ 2.0);

    WHEELS_VERIFY(k_denominator != 0.0, "Horizontal line detected");

    double k = (points_count * xy_tot - x_tot * y_tot) / k_denominator;

    double b = (y_tot - k * x_tot) / static_cast<double>(points_count);

    return std::make_pair(k, b);
}

} // namespace cmp_lattice::support
