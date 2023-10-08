#include "demo_loop.hpp"

#include <genproc/generator/tffsa/make_matrix.hpp>
#include <genproc//generator//tffsa/default_hamiltonian.hpp>
#include <genproc/processor/eigen.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>
#include <tuple>

using namespace std::chrono_literals;

namespace cmp_lattice::program {

void Greet()
{
    fmt::println("Demo of TFFSA eigenvalues computation.\n");
}

void Farewell()
{
    fmt::println("Terminating...");
}

std::string GetPath()
{
    std::string common_path;
    fmt::println("This program uses function values from files under names \"H0NS_vals.txt\", \"H0R_vals.txt\", \"Kvals.txt\", \"S_vals\".");
    fmt::println("Please enter path to the directory containing these files:");
    std::cin >> common_path;
    fmt::println("Program's output will be generated in the out.txt in the same directory.");

    return common_path;
}

auto GetLinSpace()
{
    double r_min, r_max;
    size_t points;

    fmt::println("Please enter experiment parameters: r_min, r_max, number of points:");
    std::cin >> r_min >> r_max >> points;

    return std::make_tuple(r_min, r_max, points);
}

auto GetZeta()
{
    double zeta;

    fmt::println("Please enter experiment parameters: perturbation factor:");
    std::cin >> zeta;

    return zeta;
}

auto GetEigenstatesParameters()
{
    size_t lambda;
    int momentum;

    fmt::println("Please enter experiment parameters: momentum, lambda:");
    std::cin >> momentum >> lambda;

    return std::make_pair(momentum, lambda);
}

void PrepareEnvironment(double r_min, double r_max, size_t points, size_t lambda, std::string common_path)
{
    fmt::println("Preparing experiment environment...");
    tffsa::DefaultHamiltonian::StartSeries(r_min, r_max, points, lambda, common_path);
    tffsa::StartSeries(r_min, r_max, points, lambda, common_path);   
    fmt::println("Environment is prepared."); 
}

bool ShouldDoExperiment()
{
    char ans;

    fmt::println("Do experiment [Y/n]?");
    std::cin >> ans;

    switch(ans){
        case 'Y':
            return true;
        case 'n':
            return false;
        default:
            WHEELS_PANIC("Unrecognized input!");
    }

    return false;
}

auto DoExperiment(double r_min, double, size_t, int momentum, size_t lambda, double zeta)
{
    auto start = std::chrono::steady_clock::now();

    auto matrix = tffsa::MakeMatrix<tffsa::DefaultHamiltonian>(momentum, lambda, r_min, zeta);

    auto eigen = Eigenvalues(matrix);

    auto finish = std::chrono::steady_clock::now();

    return std::make_pair(std::move(eigen), std::chrono::duration_cast<std::chrono::milliseconds>(finish - start));
}

void ReportExperimentResults(auto eigen, std::chrono::milliseconds elapsed, std::string common_path)
{
    auto path = common_path + "/out.txt";

    fmt::println("Experiment finished in {} milliseconds", elapsed.count());
    fmt::println("Results are placed at {}", path);

    std::ofstream stream(path);

    stream << eigen << std::endl;
}

void DemoProgram()
{
    Greet();

    std::string common_path = GetPath();

    while (ShouldDoExperiment()) {
        auto [r_min, r_max, points] = GetLinSpace();

        auto zeta = GetZeta();

        auto [momentum, lambda] = GetEigenstatesParameters();

        PrepareEnvironment(r_min, r_max, points, lambda, common_path);

        auto [eigen, elapsed] = DoExperiment(r_min, r_max, points, momentum, lambda, zeta);

        ReportExperimentResults(std::move(eigen), elapsed, common_path);
    }

    Farewell();
}

} // namespace cmp_lattice::program