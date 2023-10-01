#include <genproc/generator/tffsa/states_gen.hpp>

#include <genproc/generator/tffsa/make_matrix.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

using namespace cmp_lattice; // NOLINT

int main() {
    int arg1, arg2;
    double scaling;

    while (true) {
        fmt::print("Insert P -> ");

        std::cin >> arg1;

        fmt::print("Insert L -> ");

        std::cin >> arg2;

        fmt::print("Insert r -> ");

        std::cin >> scaling;

        if (scaling < 0.0) {
            break;
        }

        tffsa::StartSeries(scaling, scaling + 0.1, 2, arg2, "/home/e1ppa/thesis/cmp_lattice/src");

        auto matrix = tffsa::MakeMatrix(arg1, arg2, scaling);

        std::cout << matrix << '\n';
    }
}