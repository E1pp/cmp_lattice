#include <genproc/generator/tffsa/make_matrix.hpp>
#include <genproc//generator//tffsa/default_hamiltonian.hpp>
#include <genproc/processor/eigen.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

using namespace cmp_lattice; // NOLINT

int main() {
    int arg1, arg2;
    double scaling, zeta;

    while (true) {
        fmt::print("Insert P -> ");

        std::cin >> arg1;

        fmt::print("Insert L -> ");

        std::cin >> arg2;

        fmt::print("Insert r -> ");

        std::cin >> scaling;

        fmt::print("Insert zeta -> ");

        std::cin >> zeta;

        if (scaling < 0.0) {
            break;
        }

        auto start = std::chrono::steady_clock::now();
        std::string common_path = "/home/e1ppa/thesis/cmp_lattice/src";

        tffsa::DefaultHamiltonian::StartSeries(scaling, scaling + 0.1, 2, arg2, common_path);
        tffsa::StartSeries(scaling, scaling + 0.1, 2, arg2, common_path);

        auto matrix = tffsa::MakeMatrix<tffsa::DefaultHamiltonian>(arg1, arg2, scaling, zeta);

        auto finish = std::chrono::steady_clock::now();

        fmt::println("Matrix constructed in: {} millis", std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count());

        auto eigen = Eigenvalues(matrix);

        auto finish2 = std::chrono::steady_clock::now();

        fmt::println("Eigenvalues obtained in: {} millis", std::chrono::duration_cast<std::chrono::milliseconds>(finish2-start).count());

        std::ofstream stream("/home/e1ppa/thesis/cmp_lattice/src/out.txt");

        stream << eigen << '\n';
    }
}