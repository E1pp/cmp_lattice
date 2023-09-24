#pragma once

#include <string>
#include <vector>

namespace cmp_lattice::tffsa {

class SFunction {
public:
    SFunction(double r_min, double r_max, size_t r_n, std::string_view path);

    double operator()(double r);

private:
    const double min_;
    const double dr_;

    std::vector<double> vals_;
};

class KFunction {
public:
    KFunction(double r_min, double r_max, size_t r_n, size_t lambda, std::string_view path);

    double operator()(double r, double i);

private:
    const double min_;
    const double dr_;

    const double lambda_;

    std::vector<std::vector<double>> vals_;
};

} // namespace cmp_lattice::tffsa
