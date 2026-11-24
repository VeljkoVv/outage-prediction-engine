#include <gtest/gtest.h>
#include <chrono>
#include <stdexcept>
#include "../src/outage.h"

class OutageTest : public ::testing::Test {
    protected:
        Outage::TimePoint now = std::chrono::system_clock::now();
        Outage outage{1, ElementType::ServicePoint, 100, now};
};

TEST_F(OutageTest, NewOutagePredicted) {
    EXPECT_EQ(outage.getStatus(), OutageStatus::Predicted);
    EXPECT_TRUE(outage.isOpen());
    EXPECT_FALSE(outage.getEndedAt().has_value());
}

TEST_F(OutageTest, AddCallSkipsDuplicates) {
    outage.addCall(5);
    outage.addCall(5);

    EXPECT_EQ(outage.getCallIds().size(), 1);
}

TEST_F(OutageTest, RestoreSetsEndTime) {
    outage.restore(now);

    EXPECT_FALSE(outage.isOpen());
    EXPECT_EQ(outage.getEndedAt(), now);
}

TEST_F(OutageTest, ConfirmRestored) {
    outage.restore(now);

    EXPECT_THROW(outage.confirm(), std::logic_error);
}