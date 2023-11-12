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
static constexpr double kRmin = 0.05;
static constexpr double kRmax = 15.0;
static constexpr size_t kNumPoints = 2991; // Step = 0.005
static constexpr double kSampleRmin = 0.5;
static constexpr size_t kSampleSize = 2901; // Step = 0.005

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
    });

    return h * 1.0i;
}

void PrepareEnvironment(double r_min, double r_max, size_t points, size_t lambda, std::string common_path)
{
    fmt::println("Preparing experiment environment (r_min -> {}, r_max -> {}, points -> {}, l -> {})...", r_min, r_max, points, lambda);
    tffsa::DefaultHamiltonian::StartSeries(r_min, r_max, points, lambda, common_path);
    tffsa::StartSeries(r_min, r_max, points, lambda, common_path);   
    fmt::println("Environment is prepared."); 
}

auto DoExperiment(int momentum, size_t lambda, std::complex<double> zeta)
{
    fmt::println("Beginning experiment...");
    wheels::Defer finally([] {
        fmt::println("Experiment complete!");
    });

    static constexpr double kDelta = 0.005;

    double h = zeta.imag();
    double scaling = std::pow(/*base=*/ std::abs(h),/*pow=*/ 8 / 15);

    std::vector<double> r_data(kSampleSize, 0.0);
    std::vector<double> e1_data(kSampleSize, 0.0);
    std::vector<double> e2_data(kSampleSize, 0.0);

    double im_e1 = 0;
    double im_e2 = 0;

    for (size_t iter = 0; iter < kSampleSize; ++iter)
    {
        double r = kSampleRmin + iter * kDelta;
        r_data[iter] = r * scaling;

        auto matrix 
            = tffsa::MakeMatrix<tffsa::DefaultHamiltonian>(momentum, lambda, r, zeta);

        auto eigen = Eigenvalues(matrix);
        std::sort(eigen.begin(), eigen.end(), [] (const auto& a, const auto& b) {
            return a.real() < b.real();
        });

        im_e1 += eigen[0].imag();
        im_e2 += eigen[1].imag();

        e1_data[iter] = eigen[0].real() / scaling;
        e2_data[iter] = eigen[1].real() / scaling;
    }

    im_e1 /= static_cast<double>(kSampleSize);
    im_e2 /= static_cast<double>(kSampleSize);

    auto [k1, e01] = support::FitLine(r_data, e1_data);
    auto [k2, e02] = support::FitLine(r_data, e2_data);
    fmt::println("E1 = {} * R + {}", k1, e01);
    fmt::println("E2 = {} * R + {}", k2, e02);
    fmt::println("dK = {}, M1 = {}", k2 - k1, e02 - e01);
    fmt::println("Im(E1) = {}, Im(E2) = {}", im_e1, im_e2);
    // 0.878563
    std::vector<std::string> ans{};

    for (size_t iter = 0; iter < kSampleSize; ++iter)
    {
        std::string str{};
        str += std::to_string(r_data[iter]);
        str += " " + std::to_string(e1_data[iter]);
        str += " " + std::to_string(e2_data[iter]);
    }

    return ans;
}

void RecordResult(auto result, std::string_view path)
{
    std::ofstream stream(path);

    for (auto& iter_result : result)
    {
        stream << iter_result << std::endl;
    }
}

int main()
{
    auto [p, l] = GetEigenstatesParameters();
    PrepareEnvironment(kRmin, kRmax, kNumPoints, l, kPathToData);

    auto zeta = GetZeta();
    auto result = DoExperiment(p, l, zeta);
    RecordResult(std::move(result), kPathToData);
}
