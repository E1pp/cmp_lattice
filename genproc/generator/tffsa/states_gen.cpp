#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/support/assert.hpp>
#include <genproc/support/int_partitions.hpp>
#include <genproc/support/join.hpp>

#include <fmt/core.h>

#include <numeric>
#include <vector>

namespace cmp_lattice::tffsa {

using namespace support; // NOLINT;

using StateType = StateMaker::StateType;

StateMaker::StateMaker(int momentum, int cutoff)
    : p_(momentum),
      l_(cutoff)
{
    Assert(p_ >= 0 && l_ >= 0, "Can't use negatives yet!");
}

std::pair<std::vector<StateType>, std::vector<StateType>>
StateMaker::CreateStates() {
    return {RStates(), NSStates()};
}

bool StateMaker::IsValidEnergy(StateType& state, Sector sec) {
    int64_t local_l = sec == Sector::R ? l_ : 2 * l_;

    bool extra_check = sec == Sector::R || state.size() % 2 == 0;

    int64_t abs_sum = std::accumulate(state.begin(), state.end(), 0, [](int64_t a, int64_t b){
        return std::abs(a) + std::abs(b);
    });

    int64_t sum = std::accumulate(state.begin(), state.end(), 0);

    return extra_check && abs_sum * abs_sum - sum * sum <= local_l * local_l;
}

std::vector<StateType> StateMaker::MakeTempRStates() {
    auto boundary_checker = [this](int idx){
        return (2 * idx - p_) * (2 * idx - p_) <= p_ * p_ + l_ * l_;
    };

    std::vector<StateType> temp_states{};

    for (int i = p_; boundary_checker(i); ++i) {
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

    return temp_states;
}

// Integers that add up to p_
// and have sum of modules <= l_
std::vector<StateType> StateMaker::RStates() {
    auto temp_states = MakeTempRStates();

    std::vector<StateType> r_states{};

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

std::vector<StateType> StateMaker::MakeTempNSStates() {
    auto boundary_check = [this] (int idx) {
        return (idx - p_) * (idx - p_)  <= p_ * p_ + l_ * l_;
    };

    std::vector<StateType> temp_states{};

    for (int idx = 2 * p_; boundary_check(idx); ++idx) {
        auto left = IntegerPartitions<DupFreeOddAndInRange>::UniquePartitions(idx, {.p = p_, .l = l_});

        auto right = IntegerPartitions<DupFreeOddAndInRange>::UniquePartitions(idx - 2 * p_, {.p = p_, .l = l_});

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

std::vector<StateType> StateMaker::NSStates() {
    auto temp_states = MakeTempNSStates();

    std::vector<StateType> ns_states{};

    for (auto& state : temp_states) {
        if (IsValidEnergy(state, Sector::NS)) {
            ns_states.push_back(std::move(state));
        }
    }

    return ns_states;
}

}  // namespace cmp_lattice::tffsa