#ifndef MAKE_MATRIX_INL_H
#error \
    "Direct inclusion of this file is not allowed. Include make_matrix.hpp instead"
#include "make_matrix.hpp"
#endif

#include <include/generator/tffsa/detail/states_gen.hpp>

#include <include/support/batcher.hpp>

#include <weave/executors/thread_pool.hpp>

namespace cmp_lattice::tffsa {

namespace detail {

///////////////////////////////////////////////////////////////////////////////

class FillPerturbationMatrixTask {
 public:
  FillPerturbationMatrixTask(std::complex<double> perturb, double scale,
                             arma::Mat<std::complex<double>>& mat, size_t i,
                             size_t j, std::vector<std::vector<int>>& ns,
                             std::vector<std::vector<int>>& r);

  void operator()() noexcept;

 private:
  const std::complex<double> pert_;
  const double s_;
  arma::Mat<std::complex<double>>& matrix_;
  const size_t i_;
  const size_t j_;
  std::vector<std::vector<int>>& ns_states_;
  std::vector<std::vector<int>>& r_states_;
};

template <typename Executor>
requires std::is_base_of_v<weave::executors::IExecutor, Executor>
void MakePerturbationMatrix(std::complex<double> perturbation_factor,
                            double scaling_parameter,
                            std::vector<std::vector<int>>& ns_states,
                            std::vector<std::vector<int>>& r_states,
                            arma::Mat<std::complex<double>>& matrix,
                            Executor& exe) {
  auto num_threads = std::thread::hardware_concurrency();

  support::Batcher<FillPerturbationMatrixTask, Executor> batcher(
      exe, 4096 * num_threads);

  for (std::size_t i = 0; i < ns_states.size(); ++i) {
    for (std::size_t j = 0; j < r_states.size(); ++j) {
      batcher.Submit(FillPerturbationMatrixTask(perturbation_factor,
                                                scaling_parameter, matrix, i, j,
                                                ns_states, r_states));
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

template <Hamiltonian Base>
class FillHamiltonianMatrixTask : public Base {
 public:
  using HamiltonianPolicy = Base;

  FillHamiltonianMatrixTask(double scaling,
                            arma::Mat<std::complex<double>>& mat, size_t i,
                            size_t j, std::vector<std::vector<int>>& ns_states,
                            std::vector<std::vector<int>>& r_states)
      : Base(ns_states, r_states, scaling),
        matrix_(mat),
        i_(i),
        j_(j) {
  }

  void operator()() noexcept {
    matrix_.at(j_, i_) = Base::GetMatrixElement(i_, j_);
  }

 private:
  arma::Mat<std::complex<double>>& matrix_;
  const size_t i_;
  const size_t j_;
};

template <traits::BatcherInstance Batcher>
void MakeHamiltonianMatrix(double scaling_parameter,
                           std::vector<std::vector<int>>& ns_states,
                           std::vector<std::vector<int>>& r_states,
                           arma::Mat<std::complex<double>>& matrix,
                           traits::BatcherExecutor<Batcher>& exe) {
  using BatcherTask = traits::BatcherTask<Batcher>;
  using HamiltonianPolicy = typename BatcherTask::HamiltonianPolicy;

  const size_t size = matrix.n_rows;
  auto num_threads = std::thread::hardware_concurrency();

  Batcher batcher(exe, 4096 * num_threads);

  if constexpr (HamiltonianPolicy::IsDiagonal()) {
    for (size_t i = 0; i < size; ++i) {
      batcher.Submit(
          BatcherTask(scaling_parameter, matrix, i, i, ns_states, r_states));
    }
  } else {
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        batcher.Submit(
            BatcherTask(scaling_parameter, matrix, i, j, ns_states, r_states));
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

template <Hamiltonian HamiltonianPolicy, typename Executor>
requires std::is_base_of_v<weave::executors::IExecutor, Executor>
    arma::Mat<std::complex<double>> AsyncMakeMatrix(int momentum, int cutoff,
                                                    double scaling_parameter,
                                                    std::complex<double> perturbation_factor,
                                                    double mass,
                                                    Executor& exe) {
  auto [r_states, ns_states] = StateMaker(momentum, cutoff).CreateStates(mass);
  const size_t size = r_states.size() + ns_states.size();
  arma::Mat<std::complex<double>> matrix(size, size, arma::fill::value(0.0));

  using HamiltonianBatcherType =
      support::Batcher<FillHamiltonianMatrixTask<HamiltonianPolicy>, Executor>;

  MakeHamiltonianMatrix<HamiltonianBatcherType>(scaling_parameter, ns_states,
                                                r_states, matrix, exe);

  MakePerturbationMatrix(perturbation_factor, scaling_parameter, ns_states,
                         r_states, matrix, exe);

  return matrix;
}

}  // namespace detail

///////////////////////////////////////////////////////////////////////////////

template <Hamiltonian HamiltonianPolicy>
arma::Mat<std::complex<double>> MakeMatrix(int momentum, int cutoff,
                                           double scaling_parameter,
                                           std::complex<double> perturbation_factor,
                                           double mass) {
  auto num_threads = std::thread::hardware_concurrency();

  weave::executors::tp::fast::ThreadPool pool{num_threads};
  pool.Start();

  auto matrix = detail::AsyncMakeMatrix<HamiltonianPolicy>(
      momentum, cutoff, scaling_parameter, perturbation_factor, mass, pool);

  pool.Stop();

  return matrix;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace cmp_lattice::tffsa
