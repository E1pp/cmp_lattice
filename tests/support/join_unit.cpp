#include <include/support/join.hpp>

#include <wheels/test/framework.hpp>

using namespace cmp_lattice; // NOLINT

TEST_SUITE(Join)
{
    SIMPLE_TEST(JustWorks)
    {
        std::vector<std::vector<int>> left = {
            {1, 2},
            {3},
        };
        std::vector<std::vector<int>> right = {
            {4, 5},
            {6,},
            {7,},
        };
        auto parts = support::Join(left, right);

        static const decltype(left) kExpected = {
            {1, 2, 4, 5},
            {1, 2, 6,},
            {1, 2, 7},
            {3, 4, 5},
            {3, 6,},
            {3, 7},
        };
        ASSERT_EQ(parts.size(), kExpected.size());
        ASSERT_EQ(parts, kExpected);
    }

    SIMPLE_TEST(JoinFilter)
    {
        static struct {
            bool operator()(const std::vector<int>& vec) const
            {
                return vec.size() % 2 == 0;
            }
        } tester = {};

        std::vector<std::vector<int>> left = {
            {1, 2},
            {3},
        };
        std::vector<std::vector<int>> right = {
            {4, 5},
            {6,},
            {7,},
        };

        auto parts = support::Join(left, right, tester);
        static const decltype(parts) kExpected = {
            {1, 2, 4, 5},
        };

        ASSERT_EQ(parts, kExpected);
    }
}

RUN_ALL_TESTS()