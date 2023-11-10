#include <fmt/core.h>

#include <genproc/support/line_fitting.hpp>

using namespace cmp_lattice; // NOLINT

int main()
{
    std::vector<double> x_set
    {
        0.0,
        1.0,
        2.0,
        3.0,
        4.0,
        5.0,
    };

    std::vector<double> y_set
    {
        5.0,
        8.0,
        11.0,
        15.0,
        17.0,
        19.0,
    };

    auto [k, b] = support::FitLine(x_set, y_set);

    fmt::println("k -> {}, b-> {}", k, b);
}