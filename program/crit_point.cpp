#include <genproc/generator/tffsa/make_matrix.hpp>
#include <genproc//generator//tffsa/default_hamiltonian.hpp>

#include <genproc/processor/eigen.hpp>

#include <genproc/support/fit_line.hpp>

#include <numbers>
#include <string>
#include <fstream>

using namespace std::complex_literals;
using namespace cmp_lattice; // NOLINT

static constexpr auto kPathToData = "/home/e1ppa/thesis/cmp_lattice/data";
static constexpr double kRmin = 0.005;
static constexpr double kRmax = 15.0;
static constexpr size_t kNumPoints = 3000;

static constexpr double kSampleStart = 10.0;
static constexpr double kSampleEnd = 15.0;
static constexpr size_t kSampleSize = 100;

auto GetEigenstatesParameters()
{
    size_t lambda;
    int momentum;

    fmt::println("Please enter experiment parameters: momentum, lambda:");
    std::cin >> momentum >> lambda;

    return std::make_pair(momentum, lambda);
}

auto GetZeta()
{
    double h;

    fmt::println("Please enter experiment parameters: h (zeta = h * i):");
    std::cin >> h;

    wheels::Defer report([=] {
        fmt::println("zeta -> {}i", h);
        fmt::println("eta -> {} e^(4 i pi / 15)", std::pow(h, -8.0 / 15.0));
    });

    return h * 1.0i;
}

void PrepareEnvironment(size_t lambda, std::string common_path)
{
    fmt::println("Preparing experiment environment (r_min -> {}, r_max -> {}, points -> {}, l -> {})...", kRmin, kRmax, kNumPoints, lambda);
    tffsa::DefaultHamiltonian::StartSeries(kRmin, kRmax, kNumPoints, lambda, common_path);
    tffsa::StartSeries(kRmin, kRmax, kNumPoints, lambda, common_path);   
    fmt::println("Environment is prepared."); 
}

auto FindMass(int momentum, size_t lambda, std::complex<double> zeta)
{
    static constexpr double kDelta = (kSampleEnd - kSampleStart) / (kSampleSize - 1);

    std::vector<double> r_data(kSampleSize, 0.0);
    std::vector<double> delta_e_data(kSampleSize, 0.0);

    for (size_t iter = 0; iter < kSampleSize; ++iter)
    {
        double r = kSampleStart + iter * kDelta;
        r_data[iter] = r;

        auto matrix 
            = tffsa::MakeMatrix<tffsa::DefaultHamiltonian>(momentum, lambda, r, zeta, -1);

        auto eigen = Eigenvalues(matrix);
        std::sort(eigen.begin(), eigen.end(), [] (const auto& a, const auto& b) {
            return a.real() < b.real();
        });

        delta_e_data[iter] = eigen[1].real() - eigen[0].real();
    }

    auto [k, b] = support::FitLine(r_data, delta_e_data);

    return b;
}

auto DoExperiment(int momentum, size_t lambda)
{
    fmt::println(
        "Starting experiment (sample_start -> {}, sample_end -> {}, sample_size -> {}, l -> {})...",
        kSampleStart, 
        kSampleEnd,
        kSampleSize,
        lambda);
    auto finally = wheels::Defer([] {
        fmt::println("Experiment concluded.");
    });

    static constexpr double kMinZeta = 0.1;
    static constexpr double kMaxZeta = 0.2;
    static constexpr double kDelta = (kMaxZeta - kMinZeta) / (100);

    std::vector<std::string> entries = {};
    entries.reserve(static_cast<size_t>((kMaxZeta - kMinZeta) / kDelta) + 1);

    for (double zeta = kMinZeta; zeta < kMaxZeta; zeta += kDelta) {
        auto mass = FindMass(momentum, lambda, zeta * 1.0i);
        fmt::println("Found mass (Zeta: {}i, Mass: {})", zeta, mass);

        std::string entry = std::to_string(zeta) + "i" + " ";
        entry += std::to_string(mass);
        entries.push_back(std::move(entry));
    }

    return entries;
}

void RecordResult(auto result, std::string path)
{
    std::string common = path + "/out.txt";
    std::ofstream stream(common);
    for (auto& iter_result : result)
    {
        stream << iter_result << std::endl;
    }
}

int main()
{
    auto [p, l] = GetEigenstatesParameters();
    PrepareEnvironment(l, kPathToData);

    auto result = DoExperiment(p, l);
    RecordResult(result, kPathToData);
}
