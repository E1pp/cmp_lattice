#pragma once

#include "filter.hpp"

#include <vector>

namespace cmp_lattice::support {

namespace detail {

template <typename U>
using Vector = std::vector<U>;

struct PositiveTag {};
struct NegativeTag {};

template <Filter TFilter>
inline void CommitChanges(Vector<Vector<int>>& dest, Vector<int> insertible, TFilter& checker) {
  if (checker(insertible)) {
    dest.push_back(std::move(insertible));
  }
}

template <Filter TFilter>
inline Vector<Vector<int>> UniquePartitions(PositiveTag, int n, TFilter instance) {
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

template <Filter TFilter>
inline Vector<Vector<int>> UniquePartitions(NegativeTag, int n, TFilter instance) {
  struct TInvertedFilter {
    TFilter impl;

    bool operator()(Vector<int>& vec) {
      for (auto& num : vec) {
        num *= -1;
      }

      bool ok = impl(vec);

      for (auto& num : vec) {
        num *= -1;
      }

      return ok;        
    }
  };

  TInvertedFilter filter {
    .impl = std::move(instance)};

  auto ans = UniquePartitions(PositiveTag{}, -n, std::move(filter));

  for (auto& vec : ans) {
    for (auto& num : vec) {
      num *= -1;
    }
  }

  return ans;
}

} // namespace detail

template <Filter TFilter>
inline std::vector<std::vector<int>> UniquePartitions(int n, TFilter instance = TFilter()) {
  if (n >= 0) {
    return detail::UniquePartitions(detail::PositiveTag{}, n, instance);
  } else {
    return detail::UniquePartitions(detail::NegativeTag{}, n, instance);
  }
}

}  // namespace cmp_lattice::support