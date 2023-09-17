#pragma once

#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace cmp_lattice::tffsa {

class StateMaker {
 public:
  using StateType = std::vector<int>;

  explicit StateMaker(int momentum, int cutoff);

  std::pair<std::vector<StateType>, std::vector<StateType>> CreateStates();

 private:
  const int p_;
  const int l_;

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

  struct DupFreeOddAndInRange {
    bool operator()(StateType& vec) {
        auto odd_in_range = [this] (int num) {
            return (num % 2 == 1) &&
                   (num >= 1) &&
                   (num - p) * (num - p) <= p * p + l * l;
        };

        if(!odd_in_range(vec[0])) {
            return false;
        }

        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] == vec[i]) {
                return false;
            }

            if(!odd_in_range(vec[i])) {
                return false;
            }
        }

        return true;
    }

    const int p;
    const int l;
  };

  enum class Sector {
    NS,
    R
  };

  bool IsValidEnergy(StateType& state, Sector sector);

  std::vector<StateType> MakeTempRStates();
  std::vector<StateType> RStates();

  std::vector<StateType> MakeTempNSStates();
  std::vector<StateType> NSStates();
};

}  // namespace cmp_lattice::tffsa