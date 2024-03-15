#include <genproc/generator/tffsa/make_matrix.hpp>
#include <genproc//generator//tffsa/default_hamiltonian.hpp>
#include <genproc/processor/eigen.hpp>

#include <fmt/core.h>

#include <chrono>
#include <iostream>
#include <numbers>
#include <tuple>

using namespace std::chrono_literals;
using namespace std::complex_literals;
using namespace cmp_lattice; // NOLINT

static constexpr double kRmin = 0.005;
static constexpr double kRmax = 15.0;
static constexpr size_t kNumPoints = 3000;

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

auto GetZetaAndMass()
{
    double h;
    double m;
    fmt::println("Please enter experiment parameters: zeta = h * i. h:");
    std::cin >> h;
    fmt::println("Please enter experiment parameters: m = +-1. m:");
    std::cin >> m;

    WHEELS_VERIFY(m == 1.0 || m == -1.0, "Wrong mass!"); // NOLINT

    return std::pair(h * 1.0i, m);
}

auto GetEigenstatesParameters()
{
    size_t lambda;
    int momentum;

    fmt::println("Please enter experiment parameters: momentum, lambda:");
    std::cin >> momentum >> lambda;

    return std::make_pair(momentum, lambda);
}

auto GetMaximumEigenstates()
{
    size_t count;

    fmt::println("Please enter number of eigenstates (-1 for unlimited):");
    std::cin >> count;

    return count;
}

void PrepareEnvironment(size_t lambda, std::string common_path)
{
    fmt::println("Preparing experiment environment...");
    tffsa::DefaultHamiltonian::StartSeries(kRmin, kRmax, kNumPoints, lambda, common_path);
    tffsa::StartSeries(kRmin, kRmax, kNumPoints, lambda, common_path);   
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

    WHEELS_UNREACHABLE();

    return false;
}

auto DoExperiment(double r_min, double r_max, size_t count, int momentum, size_t lambda, std::complex<double> zeta, double mass, size_t max_eigens)
{
    fmt::println("Experiment started...");

    wheels::Defer done([]{
        fmt::println("Experiment finished.");
    });

    auto start = std::chrono::steady_clock::now();

    double delta = count == 1 ? 1 : (r_max - r_min) / (count - 1);

    std::vector<std::string> eigens{};

    for (size_t iter = 0; iter < count; ++iter)
    {
        double r = r_min + iter * delta;

        auto matrix 
            = tffsa::MakeMatrix<tffsa::DefaultHamiltonian>(momentum, lambda, r, zeta, mass);

        auto eigen = Eigenvalues(matrix);

        std::string formatted_eigens = std::to_string(r) + " ";

        for(size_t idx = 0; idx < max_eigens && idx < eigen.size(); ++idx)
        {
            formatted_eigens += std::to_string(eigen[idx].real()) + " ";
        }

        formatted_eigens.pop_back();

        eigens.push_back(std::move(formatted_eigens));
    }

    auto finish = std::chrono::steady_clock::now();

    return std::make_pair(std::move(eigens), std::chrono::duration_cast<std::chrono::milliseconds>(finish - start));
}

void ReportExperimentResults(auto eigen, std::chrono::milliseconds elapsed, std::string common_path)
{
    auto path = common_path + "/out.txt";

    fmt::println("Experiment finished in {} milliseconds", elapsed.count());
    fmt::println("Results are placed at {}", path);

    std::ofstream stream(path);

    for (auto& iter_result : eigen)
    {
        stream << iter_result << std::endl;
    }
}

int main()
{
    Greet();

    std::string common_path = GetPath();

    while (ShouldDoExperiment()) {
        auto [r_min, r_max, points] = GetLinSpace();

        auto [zeta, mass] = GetZetaAndMass();

        auto [momentum, lambda] = GetEigenstatesParameters();

        auto max_eigens = GetMaximumEigenstates();

        PrepareEnvironment(lambda, common_path);

        auto [eigen, elapsed] = DoExperiment(r_min, r_max, points, momentum, lambda, zeta, mass, max_eigens);

        ReportExperimentResults(std::move(eigen), elapsed, common_path);
    }

    Farewell();
}
