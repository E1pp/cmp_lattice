#include "states_gen.hpp"

#include <genproc/support/int_partitions.hpp>
#include <genproc/support/join.hpp>

#include <numeric>

namespace cmp_lattice::tffsa::detail {

using namespace support;  // NOLINT;

///////////////////////////////////////////////////////////////////////////////

struct DuplicateFree {
  bool operator()(std::vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
      if (vec[i - 1] == vec[i]) {
        return false;
      }
    }

    return true;
  }
};

///////////////////////////////////////////////////////////////////////////////

struct DupFreeOddAndInRange {
  bool operator()(std::vector<int>& vec) {
    auto odd_in_range = [this](int num) {
      return (num % 2 == 1) && (num >= 1) &&
             (num - p) * (num - p) <= p * p + l * l;
    };

    if (!odd_in_range(vec[0])) {
      return false;
    }

    for (size_t i = 1; i < vec.size(); ++i) {
      if (vec[i - 1] == vec[i]) {
        return false;
      }

      if (!odd_in_range(vec[i])) {
        return false;
      }
    }

    return true;
  }

  const int p;
  const int l;
};

///////////////////////////////////////////////////////////////////////////////

StateMaker::StateMaker(int momentum, int cutoff)
    : p_(momentum),
      l_(cutoff) {
}

///////////////////////////////////////////////////////////////////////////////

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
StateMaker::CreateStates() {
  return {RStates(), NSStates()};
}

///////////////////////////////////////////////////////////////////////////////

bool StateMaker::IsValidEnergy(std::vector<int>& state, Sector sec) {
  int64_t local_l = sec == Sector::R ? l_ : 2 * l_;

  bool extra_check = sec == Sector::R || state.size() % 2 == 0;

  int64_t abs_sum =
      std::accumulate(state.begin(), state.end(), 0, [](int64_t a, int64_t b) {
        return std::abs(a) + std::abs(b);
      });

  int64_t sum = std::accumulate(state.begin(), state.end(), 0);

  return extra_check && abs_sum * abs_sum - sum * sum <= local_l * local_l;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::vector<int>> StateMaker::MakeTempRStates() {
  auto boundary_checker = [this](int idx) {
    return (2 * idx - p_) * (2 * idx - p_) <= p_ * p_ + l_ * l_;
  };

  std::vector<std::vector<int>> temp_states{};

  for (int i = p_; boundary_checker(i); ++i) {
    auto left = UniquePartitions<DuplicateFree>(i);

    auto right = UniquePartitions<DuplicateFree>(i - p_);

    for (auto& vector : right) {
      for (auto& elem : vector) {
        elem *= -1;
      }
    }

    auto joined = Join(std::move(left), std::move(right));

    for (auto&& vec : joined) {
      temp_states.push_back(std::move(vec));
    }
  }

  return temp_states;
}

///////////////////////////////////////////////////////////////////////////////

// Integers that add up to p_
// and have sum of modules <= l_
std::vector<std::vector<int>> StateMaker::RStates() {
  auto temp_states = MakeTempRStates();

  std::vector<std::vector<int>> r_states{};

  for (auto& state : temp_states) {
    if (IsValidEnergy(state, Sector::R)) {
      if (state.size() % 2 != 0) {
        state.push_back(0);
      }

      r_states.push_back(std::move(state));
    }
  }

  return r_states;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::vector<int>> StateMaker::MakeTempNSStates() {
  auto boundary_check = [this](int idx) {
    return (idx - p_) * (idx - p_) <= p_ * p_ + l_ * l_;
  };

  std::vector<std::vector<int>> temp_states{};

  for (int idx = 2 * p_; boundary_check(idx); ++idx) {
    auto left = UniquePartitions<DupFreeOddAndInRange>(idx, {.p = p_, .l = l_});

    auto right = UniquePartitions<DupFreeOddAndInRange>(idx - 2 * p_,
                                                        {.p = p_, .l = l_});

    for (auto& vec : right) {
      for (auto& elem : vec) {
        elem *= -1;
      }
    }

    auto joined = Join(std::move(left), std::move(right));

    for (auto&& vec : joined) {
      temp_states.push_back(std::move(vec));
    }
  }

  return temp_states;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::vector<int>> StateMaker::NSStates() {
  auto temp_states = MakeTempNSStates();

  std::vector<std::vector<int>> ns_states{};

  for (auto& state : temp_states) {
    if (IsValidEnergy(state, Sector::NS)) {
      ns_states.push_back(std::move(state));
    }
  }

  return ns_states;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail