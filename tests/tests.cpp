#include <gtest/gtest.h>
#include <limits>
#include "syracouse_info.hpp"
#include "profile.hpp"

using namespace syracuse;

class SyracuseTest : public ::testing::Test {
protected:
    SyracuseTest() : calculator(100'000'000) {}

    SyracuseCalculator calculator;
};

TEST_F(SyracuseTest, BasicAssertions) {
    EXPECT_EQ(calculator.CalculateSequenceLength(2), 2);
    EXPECT_EQ(calculator.CalculateSequenceLength(3), 8);
    EXPECT_EQ(calculator.CalculateSequenceLength(4), 3);
    EXPECT_EQ(calculator.CalculateSequenceLength(5), 6);
    EXPECT_EQ(calculator.CalculateSequenceLength(6), 9);
    EXPECT_EQ(calculator.CalculateSequenceLength(7), 17);
    EXPECT_EQ(calculator.CalculateSequenceLength(8), 4);
    EXPECT_EQ(calculator.CalculateSequenceLength(9), 20);
    EXPECT_EQ(calculator.CalculateSequenceLength(10), 7);
    EXPECT_EQ(calculator.CalculateSequenceLength(13), 10);
    EXPECT_EQ(calculator.CalculateSequenceLength(27), 112);
}

TEST_F(SyracuseTest, OverflowDetection) {
    constexpr uint64_t large_value{std::numeric_limits<uint64_t>::max() / 2};
    
    EXPECT_THROW(calculator.CalculateSequenceLength(large_value), std::overflow_error);
    
    constexpr uint64_t max_safe{(std::numeric_limits<uint64_t>::max() - 1) / 3};
    EXPECT_NO_THROW(calculator.CalculateSequenceLength(max_safe));
}

TEST_F(SyracuseTest, EdgeCases) {
    EXPECT_EQ(calculator.CalculateSequenceLength(0), 0);
    EXPECT_EQ(calculator.CalculateSequenceLength(1), 1);
}

// TEST_F(SyracuseTest, LongestSequence) {
//     SyracuseInfo result = calculator.FindLongestSequence(700'000'000);
//     EXPECT_EQ(result.start_value, 670617279);
//     EXPECT_EQ(result.sequence_length, 987);
// }

class SyracusePralelTest : public ::testing::Test {
protected:
    SyracusePralelTest() : calculator(1'000'000'000) {}

    SyracuseCalculator calculator;

    const uint64_t THREADS_AMOUNT{12};
};

TEST_F(SyracusePralelTest, SyracusePralelTest1M) {
    LOG_DURATION("Threaded 1M test");
    SyracuseInfo result = calculator.FindLongestSequenceParalel(1'000'000, THREADS_AMOUNT);
    EXPECT_EQ(result.start_value, 837799);
    EXPECT_EQ(result.sequence_length, 525);
}

TEST_F(SyracusePralelTest, SyracusePralelTest10M) {
    LOG_DURATION("Threaded 10M test");
    SyracuseInfo result = calculator.FindLongestSequenceParalel(10'000'000, THREADS_AMOUNT);
    EXPECT_EQ(result.start_value, 8400511);
    EXPECT_EQ(result.sequence_length, 686);
}

TEST_F(SyracusePralelTest, SyracusePralelTest100M) {
    LOG_DURATION("Threaded 100M test");
    SyracuseInfo result = calculator.FindLongestSequenceParalel(100'000'000, THREADS_AMOUNT);
    EXPECT_EQ(result.start_value, 63728127);
    EXPECT_EQ(result.sequence_length, 950);
}

TEST_F(SyracusePralelTest, PralelTest200M) {
    SyracuseInfo result{};
    {
        LOG_DURATION("LargeRangeTest Duration for range up to 200'000'000");
        result = calculator.FindLongestSequenceParalel(200'000'000, THREADS_AMOUNT);
    }
    EXPECT_EQ(result.start_value, 169941673);
    EXPECT_EQ(result.sequence_length, 954);
}

TEST_F(SyracusePralelTest, PralelTestExpected700M) {
    SyracuseInfo result{};
    {
        LOG_DURATION("LargeRangeTest Duration for range up to 700'000'000");
        result = calculator.FindLongestSequenceParalel(700'000'000, THREADS_AMOUNT);
    }
    EXPECT_EQ(result.start_value, 670617279);
    EXPECT_EQ(result.sequence_length, 987);
}

TEST_F(SyracusePralelTest, PralelTestExpected1B) {
    SyracuseInfo result{};
    {
        LOG_DURATION("LargeRangeTest Duration for range up to 1'000'000'000");
        result = calculator.FindLongestSequenceParalel(1'000'000'000, THREADS_AMOUNT);
    }
    EXPECT_EQ(result.start_value, 670617279);
    EXPECT_EQ(result.sequence_length, 987);
}

// TEST_F(SyracusePralelTest, PralelTestExpected10B) {
//     SyracuseInfo result{};
//     {
//         LOG_DURATION("LargeRangeTest Duration for range up to 10'000'000'000");
//         result = calculator.FindLongestSequenceParalel(10'000'000'000, THREADS_AMOUNT);
//     }
//     EXPECT_EQ(result.start_value, 9780657630);
//     EXPECT_EQ(result.sequence_length, 1133);
// }
