#pragma once

#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace cmp_lattice::tffsa {

class StateMaker {
 public:
  using StateType = std::vector<int>;

  struct DuplicateFree {
    bool operator()(StateType& vec) {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] == vec[i]) {
                return false;
            }
        }

        return true;
    }
  };

  explicit StateMaker(int momentum, int cutoff);

  std::pair<std::vector<StateType>, std::vector<StateType>> CreateStates();

 private:
  const int p_;
  const int l_;

  std::vector<StateType> RStates();

  void NSStates(int curr_p, int curr_l);
};

}  // namespace cmp_lattice::tffsa