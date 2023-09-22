#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/generator/tffsa//trig_factors.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>
#include "genproc/generator/tffsa/sectors.hpp"

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

        for (auto& ns_state : ns_states) {
            for (auto& r_state : r_states) {                
                double ns_tan_factor = tffsa::TanFactor(ns_state, tffsa::Sector::NS, 1.0);
                double r_tan_factor = tffsa::TanFactor(r_state, tffsa::Sector::R, 1.0);
                double cot_factor = tffsa::CotFactor(tffsa::NSState(ns_state), tffsa::RState(r_state), 1.0);

                fmt::println("NS Tan factor -> {}", ns_tan_factor);
                fmt::println("R Tan factor -> {}", r_tan_factor);
                fmt::println("Cot factor -> {}", cot_factor);
            }
        }

    }


}