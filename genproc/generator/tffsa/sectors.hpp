#pragma once

#include <vector>

namespace cmp_lattice::tffsa {

enum class Sector {
    NS,
    R
};

struct NSState {
    explicit NSState(const std::vector<int>& vec) : impl(vec) {}

    const std::vector<int>& impl;
};

struct RState {
    explicit RState(const std::vector<int>& vec) : impl(vec) {}

    const std::vector<int>& impl;
};

} // namespace cmp_lattice::tffsa
