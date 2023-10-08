#pragma once

#include <string>
#include <vector>

namespace cmp_lattice::support {

class FunctionFromFile {
public:
    FunctionFromFile(double r_min, double r_max, size_t r_n, std::string_view path);

    double operator()(double r);

private:
    const double r_min_;
    const double dr_;

    std::vector<double> vals_;
};

} // namespace cmp_lattice::support
