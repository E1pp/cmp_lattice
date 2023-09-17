#pragma once

#include <armadillo>
#include <fmt/core.h>

namespace cmp_lattice {

class Tester {
 public:
  void Test() const {
    arma::Mat<int> a(2, 2);
    a.fill(5);

    for (auto elem : a) {
      fmt::print("{} ", elem);
    }

    fmt::println("Imagine testing your software");
  }
};

}  // namespace cmp_lattice