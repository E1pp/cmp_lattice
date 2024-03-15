#include <genproc/support/fit_line.hpp>

#include <wheels/test/framework.hpp>

using namespace cmp_lattice; // NOLINT

TEST_SUITE(FitLine)
{
    SIMPLE_TEST(JustWorks)
    {
        std::vector<double> x {
            1,
            2,
            3,
            4,
            5,
            6,
        };
        std::vector<double> y {
            5,
            10,
            15,
            20,
            25,
            30,
        };
        auto [k, b] = support::FitLine(x, y);
        ASSERT_EQ(k, 5.0);
        ASSERT_EQ(b, 0.0);
    }

    SIMPLE_TEST(BadData)
    {
        std::vector<double> x {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9,
            10,
            14,
        };
        std::vector<double> y {
            5,
            10,
            15,
            20,
            25,
            30,
            35,
            40,
            45,
            50,
            55,
        };
        static constexpr double kMargin = 0.001;

        auto [k, b] = support::FitLine(x, y);
        ASSERT_TRUE(std::abs(k - 4.218) <= kMargin);
        ASSERT_TRUE(std::abs(b - 3.543) <= kMargin);
    }

    SIMPLE_TEST(VeryBadData)
    {
        std::vector<double> x {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9,
            10,
            14,
        };
        std::vector<double> y {
            5,
            10,
            15,
            20,
            25,
            0,
            35,
            40,
            45,
            50,
            55,
        };
        static constexpr double kMargin = 0.001;

        auto [k, b] = support::FitLine(x, y);
        ASSERT_TRUE(std::abs(k - 4.273) <= kMargin);
        ASSERT_TRUE(std::abs(b - 0.469) <= kMargin);
    }
}

RUN_ALL_TESTS()