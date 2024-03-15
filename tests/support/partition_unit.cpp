#include <genproc/support/int_partitions.hpp>

#include <wheels/test/framework.hpp>

using namespace cmp_lattice; // NOLINT

TEST_SUITE(Partitions)
{
    SIMPLE_TEST(JustWorks)
    {
        auto parts = support::UniquePartitions<support::Truer>(5);
        static const std::vector<std::vector<int>> kExpected = {
            {5,},
            {4, 1},
            {3, 2},
            {3, 1, 1},
            {2, 2, 1},
            {2, 1, 1, 1},
            {1, 1, 1, 1, 1},
        };
        ASSERT_EQ(kExpected.size(), parts.size());
        
        for (size_t idx = 0; idx < parts.size(); ++idx) {
            ASSERT_EQ(kExpected[idx], parts[idx]);
        }
    }

    SIMPLE_TEST(NegativeNumber)
    {
        auto parts = support::UniquePartitions<support::Truer>(-5);
        static const std::vector<std::vector<int>> kExpected = {
            {-5,},
            {-4, -1},
            {-3, -2},
            {-3, -1, -1},
            {-2, -2, -1},
            {-2, -1, -1, -1},
            {-1, -1, -1, -1, -1},
        };
        ASSERT_EQ(kExpected.size(), parts.size());

        for (size_t idx = 0; idx < parts.size(); ++idx) {
            ASSERT_EQ(kExpected[idx], parts[idx]);
        }
    }

    SIMPLE_TEST(Filter)
    {
        struct TestFilter
        {
            bool operator()(const std::vector<int>& f) const
            {
                int tot = 1;

                for (const auto& val : f) {
                    tot *= val;

                    if (tot > 5) {
                        return false;
                    }
                }

                return true;
            }
        } tester = {};

        auto parts = support::UniquePartitions(5, tester);
        static const std::vector<std::vector<int>> kExpected = {
            {5, },
            {4, 1},
            {3, 1, 1},
            {2, 2, 1},
            {2, 1, 1, 1},
            {1, 1, 1, 1, 1},
        };
        ASSERT_EQ(kExpected.size(), parts.size());
        
        for (size_t idx = 0; idx < parts.size(); ++idx) {
            ASSERT_EQ(kExpected[idx], parts[idx]);
        }
    }

    SIMPLE_TEST(NegativeFilter)
    {
        struct TestFilter
        {
            bool operator()(const std::vector<int>& f) const
            {
                int tot = 1;

                for (const auto& val : f) {
                    tot *= val;
                }

                return tot < 4;
            }
        } tester = {};

        auto parts = support::UniquePartitions(-5, tester);
        static const std::vector<std::vector<int>> kExpected = {
            {-5,},
            {-3, -1, -1},
            {-2, -2, -1},
            {-2, -1, -1, -1},
            {-1, -1, -1, -1, -1},
        };
        ASSERT_EQ(kExpected.size(), parts.size());
        
        for (size_t idx = 0; idx < parts.size(); ++idx) {
            ASSERT_EQ(kExpected[idx], parts[idx]);
        }
    }
}

RUN_ALL_TESTS()