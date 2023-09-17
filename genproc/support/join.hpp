#pragma once

#include "filter.hpp"

#include <vector>

#include <fmt/core.h>

namespace cmp_lattice::support {

template <Filter TFilter = Truer>
inline std::vector<std::vector<int>> Join(std::vector<std::vector<int>> left, std::vector<std::vector<int>> right, TFilter checker = TFilter()) {
    std::vector<std::vector<int>> ans{};

    for (size_t i = 0; i < left.size(); ++i) {
        if (!checker(left[i])) {
            continue;
        }

        for (size_t j = 0; j < right.size(); ++j) {
            auto l_copy = left[i];

            if (checker(right[j])) {
                l_copy.insert(l_copy.end(), right[j].begin(), right[j].end());

                ans.push_back(std::move(l_copy));
            }
        }
    }

    return ans;
}

}  // namespace cmp_lattice::support