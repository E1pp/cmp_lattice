#pragma once

#include "filter.hpp"

#include <vector>

namespace cmp_lattice::support {

template <Filter TFilter>
class IntegerPartitions {
 public:
  template <typename U>
  using Vector = std::vector<U>;

  static Vector<Vector<int>> UniquePartitions(int n, TFilter instance) {
    if (n == 0) {
        return {{}};
    }
    
    Vector<Vector<int>> ans{};

    Vector<int> numbers(n, 0);

    int idx = 0;

    numbers[idx] = n;

    while (true) {
      CommitChanges(ans,
                    Vector<int>(numbers.begin(), numbers.begin() + idx + 1), instance);
      
      int remainder = 0;

      while (idx >= 0 && numbers[idx] == 1) {
        remainder++;
        idx--;
      }

      if (idx < 0) {
        break;
      }

      numbers[idx]--;
      remainder++;

      while (remainder > numbers[idx]) {
        numbers[idx + 1] = numbers[idx];

        remainder -= numbers[idx];

        idx++;
      }

      numbers[++idx] = remainder;
    }

    return ans;
  }

 private:
  static void CommitChanges(Vector<Vector<int>>& dest, Vector<int> insertible, TFilter& checker) {
    if (checker(insertible)) {
      dest.push_back(std::move(insertible));
    }
  }
};

}  // namespace cmp_lattice::support