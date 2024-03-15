#pragma once

#include <genproc/generator/tffsa/sectors.hpp>

#include <tuple>
#include <vector>

namespace cmp_lattice::tffsa::detail {

///////////////////////////////////////////////////////////////////////////////

class StateMaker {
 public:
  explicit StateMaker(int momentum, int cutoff);

  // RStates, NSStates
  std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
  CreateStates(double mass);

 private:
  const int p_;
  const int l_;

  bool IsValidEnergy(std::vector<int>& state, Sector sector);

  std::vector<std::vector<int>> MakeTempRStates();
  std::vector<std::vector<int>> RStates(double mass);

  std::vector<std::vector<int>> MakeTempNSStates();
  std::vector<std::vector<int>> NSStates();
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa::detail