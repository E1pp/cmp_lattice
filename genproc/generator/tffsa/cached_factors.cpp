#include "cached_factors.hpp"

#include <cassert>
#include <fstream>

#include <fmt/core.h>

namespace cmp_lattice::tffsa {

SFunction::SFunction(double r_min,
    double r_max,
    size_t r_n,
    std::string_view path)
    : min_(r_min)
    , dr_((r_max - r_min) / static_cast<double>(r_n - 1))
{
    vals_.reserve(r_n);

    std::ifstream stream(path);

    for (size_t i = 0; i < r_n ; ++i) {
        double var = 0;

        stream >> var;

        vals_.push_back(var);

        fmt::println("{}", var);
    }
}

double SFunction::operator()(double r)
{
    size_t pos = (r - min_) / dr_;

    assert(pos >= 0 && pos <= vals_.size());

    return vals_[pos];
}

KFunction::KFunction(double r_min,
    double r_max,
    size_t r_n,
    size_t lambda,
    std::string_view path)
    : min_(r_min)
    , dr_((r_max - r_min) / static_cast<double>(r_n - 1))
    , lambda_(lambda)
{
    std::ifstream stream(path);

    vals_.reserve(r_n);

    for (size_t i = 0; i < r_n; ++i) {
        std::vector<double> row(2 * lambda_ + 1);

        for (auto& val : row) {
            stream >> val;
        }

        vals_.push_back(std::move(row));
    }
}

double KFunction::operator()(double r, double i)
{
    size_t r_idx = (r - min_) / dr_;

    assert(r_idx >= 0 && r_idx < vals_.size());

    size_t vec_idx = 2 * i;

    assert(vec_idx >= 0 && vec_idx <= 2 * lambda_);

    return vals_[r_idx][vec_idx];
}


} // namespace cmp_lattice::tffsa