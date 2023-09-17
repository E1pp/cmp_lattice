#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/support/int_partitions.hpp>
#include <genproc/support/join.hpp>

#include <fmt/core.h>

#include <numeric>

namespace cmp_lattice::tffsa {

using namespace support; // NOLINT;

using StateType = StateMaker::StateType;

StateMaker::StateMaker(int momentum, int cutoff)
    : p_(momentum),
      l_(cutoff)
{ }

std::pair<std::vector<StateType>, std::vector<StateType>>
StateMaker::CreateStates() {
    return {RStates(), {}};
}

// Integers that add up to p_
// and have sum of modules <= l_
std::vector<StateType> StateMaker::RStates() {
    auto bound_checker = [this](int idx){
        return (2 * idx - p_) * (2 * idx - p_) <= p_ * p_ + l_ * l_;
    };

    std::vector<StateType> temp_states{};

    for (int i = p_; bound_checker(i); ++i) {
        auto left = IntegerPartitions<DuplicateFree>::UniquePartitions(i, DuplicateFree());

        auto right = IntegerPartitions<DuplicateFree>::UniquePartitions(i - p_, DuplicateFree());

        for (auto& vector : right) {
            for (auto& elem : vector) {
                elem *= -1;
            }
        }

        auto joined = Join(std::move(left), std::move(right));

        for(auto&& vec : joined) {
            temp_states.push_back(std::move(vec));
        }
    }

    std::vector<StateType> r_states{};

    auto valid_energy = [this] (StateType& state) {
        int64_t abs_sum = std::accumulate(state.begin(), state.end(), 0, [](int64_t a, int64_t b){
            return std::abs(a) + std::abs(b);
        });

        int64_t sum = std::accumulate(state.begin(), state.end(), 0);

        return abs_sum * abs_sum - sum * sum <= l_ * l_;
    };

    for (auto& state : temp_states) {
        if (valid_energy(state)) {
            if (state.size() % 2 != 0) {
                state.push_back(0);
            }

            r_states.push_back(std::move(state));
        }
    }

    return r_states;
}

// void StateMaker::NSStates(int curr_p, int curr_l) {

// }

}  // namespace cmp_lattice::tffsa