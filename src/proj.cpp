#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/generator/tffsa//trig_factors.hpp>

#include <genproc/generator/tffsa/cached_factors.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

using namespace cmp_lattice; // NOLINT

int main() {
    int arg1, arg2;

    while (true) {
        fmt::print("Insert P -> ");

        std::cin >> arg1;

        fmt::print("Insert L -> ");

        std::cin >> arg2;

        // if(arg1 < 0 || arg2 < 0) {
        //     break;
        // }

        tffsa::StateMaker maker(arg1, arg2);

        auto start = std::chrono::steady_clock::now();

        auto [r_states, ns_states] = maker.CreateStates();

        auto finish = std::chrono::steady_clock::now();

        fmt::println("Time elapsed: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count());

        fmt::println("RStates: [");
        for (auto& vec : r_states) {
            fmt::print("[ ");
            for(auto num : vec){
                fmt::print("{} ", num);
            }

            fmt::println("]");
        }
        fmt::println("]");

        fmt::println("NSStates: [");
        for (auto& vec : ns_states) {
            fmt::print("[ ");
            for(auto num : vec){
                fmt::print("{}/2 ", num);
            }

            fmt::println("]");
        }
        fmt::println("]");
    }


}