#include <genproc/support/function_from_file.hpp>

#include <wheels/test/framework.hpp>

#include <fstream>

using namespace cmp_lattice; // NOLINT

TEST_SUITE(Function1D)
{
    static constexpr auto kPath = "test_data.txt";

    SIMPLE_TEST(JustWorks)
    {
        std::ofstream out(kPath);
        out << 5.5 << ' ' << 6.0 << '\n';
        out.close();

        support::FunctionFromFile func(0.0, 5.0, 2, kPath);
        ASSERT_EQ(func(0.0), 5.5);
        ASSERT_EQ(func(1.0), 5.5);
        ASSERT_EQ(func(3.0), 5.5);
        ASSERT_EQ(func(4.9999), 5.5);
        ASSERT_EQ(func(5.0), 6.0);
    }

    SIMPLE_TEST(RemainderOfFile)
    {
        std::ofstream out(kPath);
        out << 5.5 << ' ' << 6.0 << ' ' << "garbage\n";
        out.close();

        support::FunctionFromFile func(0.0, 5.0, 2, kPath);
        ASSERT_EQ(func(0.0), 5.5);
        ASSERT_EQ(func(1.0), 5.5);
        ASSERT_EQ(func(3.0), 5.5);
        ASSERT_EQ(func(4.9999), 5.5);
        ASSERT_EQ(func(5.0), 6.0);
    }
}

RUN_ALL_TESTS()