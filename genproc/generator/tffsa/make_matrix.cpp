#include "make_matrix.hpp"

#include <genproc/generator/tffsa/detail/cached_factors.hpp>
#include <genproc/generator/tffsa/detail/form_factor.hpp>
#include <genproc/generator/tffsa/detail/sector_factors.hpp>

#include <weave/executors/thread_pool.hpp>

#include <complex>
#include <optional>
#include <string>

namespace cmp_lattice::tffsa {

///////////////////////////////////////////////////////////////////////////////

static std::optional<detail::FunctionFromFile> cached_function;

///////////////////////////////////////////////////////////////////////////////

void StartSeries(double r_min, double r_max, size_t r_n, size_t lambda,
                 std::string common_path) {
  if (cached_function) {
    return;
  }

  std::string s_func = common_path + "/S_vals.txt";
  std::string k_func = common_path + "/K_vals.txt";

  cached_function.emplace(r_min, r_max, r_n, s_func);
  detail::StartSeries(r_min, r_max, r_n, lambda, k_func);
}

///////////////////////////////////////////////////////////////////////////////

namespace detail {

std::complex<double> MatrixElement(NSState ns_state, RState r_state,
                                   double scaling_factor) {
  assert(cached_function.has_value());

  return FormFactor(ns_state, r_state, scaling_factor) *
         detail::StateFactor(scaling_factor, Sector::NS, ns_state.impl) *
         detail::StateFactor(scaling_factor, Sector::R, r_state.impl) *
         (*cached_function)(scaling_factor);
}

///////////////////////////////////////////////////////////////////////////////

FillPerturbationMatrixTask::FillPerturbationMatrixTask(
    std::complex<double> perturb, double scale, arma::Mat<std::complex<double>>& mat,
    size_t i, size_t j, std::vector<std::vector<int>>& ns,
    std::vector<std::vector<int>>& r)
    : pert_(perturb),
      s_(scale),
      matrix_(mat),
      i_(i),
      j_(j),
      ns_states_(ns),
      r_states_(r) {
}

void FillPerturbationMatrixTask::operator()() noexcept {
  auto element =
      s_ * MatrixElement(NSState(ns_states_[i_]), RState(r_states_[j_]), s_);

  matrix_.at(i_, ns_states_.size() + j_) = element * pert_;
  matrix_.at(ns_states_.size() + j_, i_) = std::conj(element) * pert_;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace detail

}  // namespace cmp_lattice::tffsa