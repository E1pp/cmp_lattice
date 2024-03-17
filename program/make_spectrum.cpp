#include <include/processor/tffsa/spectrum.hpp>

#include <string>
#include <numbers>

using namespace std::complex_literals; // NOLINT
using namespace cmp_lattice; // NOLINT

std::complex<double> MakeZeta(double abs, double phase)
{
    return abs * std::exp(1.0i * std::numbers::pi * phase);
}

tffsa::GenerateSpectrumParameters MakeParams(char* argv[]) {
    return tffsa::GenerateSpectrumParameters{
        .cached = tffsa::CachedFunctionsParameters{
            .cached_grid = tffsa::GridParameters{
                .r_min = std::stod(argv[1]),
                .r_max = std::stod(argv[2]),
                .points_count = static_cast<size_t>(std::stoi(argv[3])),
            },
            .lambda = std::stoi(argv[8]),
        },
        .matrix = tffsa::MatrixSerialParameters{
            .def = tffsa::MatrixGenerationParameters{
                .momentum = std::stoi(argv[7]),
                .lambda = std::stoi(argv[8]),
                .scaling = 0,
                .zeta = MakeZeta(std::stod(argv[9]), std::stod(argv[10])),
                .mass = std::stod(argv[11]),
            },
            .grid = tffsa::GridParameters{
                .r_min = std::stod(argv[4]),
                .r_max = std::stod(argv[5]),
                .points_count = static_cast<size_t>(std::stoi(argv[6])),
            },
        },
    };
}

int main(int argc, char* argv[]) {
    if (argc < 13) {
        std::cerr << "Usage: " << argv[0] << " [cached_grid_r_min] [cached_grid_r_max] [cached_grid_points_count] [r_min] [r_max] [points_count] [momentum] [lambda] [zeta_abs] [zeta_phase] [mass] [eigen_count] [(optional) path_to_data_folder] [(optional) path_to_out_folder]" << '\n';
        return -1;
    }

    auto params = MakeParams(argv);

    int eigen_count = std::stoi(argv[12]);

    std::filesystem::path path = std::filesystem::current_path();
    if (argc >= 14) {
        path = argv[13];
    }

    auto data = tffsa::GenerateSpectrum(params, path);

    auto out_path = path /= "out.txt";
    if (argc >= 15) {
        out_path = argv[14];
    }
    std::ofstream stream(out_path);
    for (const auto& exp_step : data) {
        std::string row{};
        
        int count = 0;
        for (const auto& val : exp_step) {
            row += std::to_string(val.real()) + " ";
            if (++count == eigen_count) {
                break;
            }
        }
        row.pop_back();

        stream << row << '\n';
    }

    return 0;
}