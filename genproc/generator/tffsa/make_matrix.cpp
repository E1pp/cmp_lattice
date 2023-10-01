#include "make_matrix.hpp"

#include "cached_factors.hpp"
#include "form_factor.hpp"
#include "sector_factors.hpp"
#include "states_gen.hpp"

#include <genproc/support/batcher.hpp>

#include <cassert>
#include <complex>
#include <optional>
#include <thread>
#include <string>

#include <fmt/core.h>

#include <weave/executors/thread_pool.hpp>

namespace cmp_lattice::tffsa {

/////////////////////////////////////////////////////////////////////////////////////////////////

static std::optional<SFunction> cached_function;

void StartSeries(double r_min,
    double r_max,
    size_t r_n,
    size_t lambda,
    std::string common_path)
{
    std::string s_func = common_path + "/Stest_vals.txt";
    std::string k_func = common_path + "/Ktest_vals.txt";

    cached_function.emplace(r_min, r_max, r_n, s_func);
    sector_factors::StartSeries(r_min, r_max, r_n, lambda, k_func);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

std::complex<double> MatrixElement(NSState ns_state, RState r_state, double scaling_factor)
{
    assert(cached_function.has_value());

    return FormFactor(ns_state, r_state, scaling_factor) *
           sector_factors::StateFactor(scaling_factor, Sector::NS, ns_state.impl) *
           sector_factors::StateFactor(scaling_factor, Sector::R, r_state.impl) *
           (*cached_function)(scaling_factor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

struct FillMatrixTask
{
public:
    FillMatrixTask(double scale,
                   std::complex<double>& mat,
                   std::vector<int>& ns,
                   std::vector<int>& r)
        : s_(scale)
        , ref_(mat)
        , ns_state_(ns)
        , r_state_(r)
    { }

    void operator()() noexcept
    {
        ref_ = MatrixElement(NSState(ns_state_), RState(r_state_), s_);
    }
    
private:
    double s_;
    std::complex<double>& ref_;
    std::vector<int>& ns_state_;
    std::vector<int>& r_state_;
};

template <typename Executor>
arma::Mat<std::complex<double>> MakeHelperMatrix(int momentum, int cutoff, double scaling_parameter, Executor& exe)
{
    auto [r_states, ns_states] = StateMaker(momentum, cutoff).CreateStates();

    arma::Mat<std::complex<double>> helper_matrix(ns_states.size(), r_states.size(), arma::fill::value(0.0));

    auto num_threads = std::thread::hardware_concurrency();

    support::Batcher<FillMatrixTask, Executor> batcher(exe, 4096 * num_threads);

    for (std::size_t i = 0; i < ns_states.size(); ++i) {
        for (std::size_t j = 0; j < r_states.size(); ++j) {
            batcher.Submit(FillMatrixTask(
                scaling_parameter,
                helper_matrix.at(i, j),
                ns_states[i],
                r_states[j]));
        }
    }

    return helper_matrix;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

std::size_t ComputeDimensions(std::size_t r_count, std::size_t ns_count)
{
    return r_count + ns_count;
}

// static std::size_t top_right_top_left = 0;
// static std::size_t top_right_bot_right = 0;

// void EstablishTopRight(std::size_t r_count, std::size_t ns_count)
// {

// }

// bool IsInTopRight(std::size_t idx1, std::size_t idx2)
// {

// }

// static std::size_t bot_left_top_left = 0;
// static std::size_t bot_left_bot_right = 0;

// void EstablishBotLeft(std::size_t r_count, std::size_t ns_count)
// {

// }

// bool IsInBotLeft(std::size_t idx1, std::size_t idx2)
// {

// }

/////////////////////////////////////////////////////////////////////////////////////////////////

arma::Mat<std::complex<double>> MakeMatrix(int momentum, int cutoff, double scaling_parameter)
{
    auto num_threads = std::thread::hardware_concurrency();
    
    weave::executors::tp::fast::ThreadPool pool{num_threads};
    pool.Start();

    auto helper_matrix = MakeHelperMatrix(momentum, cutoff, scaling_parameter, pool);

    size_t size = ComputeDimensions(helper_matrix.n_cols, helper_matrix.n_rows);

    try {
        arma::Mat<std::complex<double>> matrix(size, size, arma::fill::value(0.0));
    } catch (...) {
        std::cout << "This is too much -> " << size << '\n';
    }



    pool.Stop();

    return helper_matrix;
}

} // cmp_lattice::tffsa