#include "make_matrix.hpp"

#include "cached_factors.hpp"
#include "form_factor.hpp"
#include "sector_factors.hpp"
#include "states_gen.hpp"

#include <cassert>
#include <complex>
#include <optional>
#include <thread>
#include <string>

#include <fmt/core.h>

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

    // std::cout << (FormFactor(ns_state, r_state, scaling_factor) *
    //        sector_factors::StateFactor(scaling_factor, Sector::NS, ns_state.impl) *
    //        sector_factors::StateFactor(scaling_factor, Sector::R, r_state.impl) *
    //        (*cached_function)(scaling_factor)) << '\n';

    return FormFactor(ns_state, r_state, scaling_factor) *
           sector_factors::StateFactor(scaling_factor, Sector::NS, ns_state.impl) *
           sector_factors::StateFactor(scaling_factor, Sector::R, r_state.impl) *
           (*cached_function)(scaling_factor);
}

std::size_t ComputeDimensions(std::size_t r_count, std::size_t ns_count)
{
    return r_count + ns_count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

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
    auto [r_states, ns_states] = StateMaker(momentum, cutoff).CreateStates();

    // std::size_t size = ComputeDimensions(r_states.size(), ns_states.size());

    arma::Mat<std::complex<double>> helper_matrix(ns_states.size(), r_states.size(), arma::fill::value(0.0));
    
    std::vector<std::thread> jobs;
    jobs.reserve(ns_states.size() * r_states.size());

    for (std::size_t i = 0; i < ns_states.size(); ++i) {
        for (std::size_t j = 0; j < r_states.size(); ++j) {
            // helper_matrix.at(i, j) = MatrixElement(NSState(ns_states[i]), RState(r_states[j]), scaling_parameter);
            jobs.emplace_back(
                [scaling_parameter,
                 &mat_ref = helper_matrix.at(i, j),
                 &ns_state = ns_states[i],
                 &r_state = r_states[j]
                 ]{
                    mat_ref = MatrixElement(NSState(ns_state), RState(r_state), scaling_parameter);
                });
        }
    }

    for (auto& job : jobs) {
        job.join();
    }

    jobs.clear();

    // arma::Mat<std::complex<double>> matrix(size, size, arma::fill::value(0.0));

    return helper_matrix;
}

} // cmp_lattice::tffsa