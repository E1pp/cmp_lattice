#pragma once

#include <vector>

namespace cmp_lattice::support {

template <typename T>
concept Filter = requires(T filter, std::vector<int>& filtrable) {
  { filter(filtrable) } -> std::same_as<bool>;
};

struct Truer {
  bool operator()(std::vector<int>&) {
    return true;
  }
};

}  // namespace cmp_lattice::support