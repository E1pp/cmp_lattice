#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/support/int_partitions.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

using namespace cmp_lattice; // NOLINT

struct Tester {
  bool operator()(std::vector<int>& vec){
    for(size_t i = 1; i < vec.size(); ++i){
        if (vec[i - 1] == vec[i]) {
            return false;
        }
    }

    return true;
  }
};

int main() {
    int arg1, arg2;

    while (true) {
        fmt::print("Insert P -> ");

        std::cin >> arg1;

        fmt::print("Insert L -> ");

        std::cin >> arg2;

        if(arg1 < 0 || arg2 < 0) {
            break;
        }

        tffsa::StateMaker maker(arg1, arg2);

        auto start = std::chrono::steady_clock::now();

        auto [v1, v2] = maker.CreateStates();

        auto finish = std::chrono::steady_clock::now();

        fmt::println("Time elapsed: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count());

        fmt::println("[");
        for (auto& vec : v2) {
            fmt::print("[ ");
            for(auto num : vec){
                fmt::print("{}/2 ", num);
            }

            fmt::println("]");
        }
        fmt::println("]");
    }


}