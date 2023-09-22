#pragma once

#include "sectors.hpp"

#include <tuple>
#include <vector>

namespace cmp_lattice::tffsa {

class StateMaker {
 public:
  using StateType = std::vector<int>;

  explicit StateMaker(int momentum, int cutoff);
  
  // RStates, NSStates
  std::pair<std::vector<StateType>, std::vector<StateType>> CreateStates();

 private:
  const int p_;
  const int l_;

  bool IsValidEnergy(StateType& state, Sector sector);

  std::vector<StateType> MakeTempRStates();
  std::vector<StateType> RStates();

  std::vector<StateType> MakeTempNSStates();
  std::vector<StateType> NSStates();
};

}  // namespace cmp_lattice::tffsa