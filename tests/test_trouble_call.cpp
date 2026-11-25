#include <gtest/gtest.h>
#include "../src/trouble_call.h"

TEST(TroubleCallTest, NewCallOpen) {
    Trouble_Call call(1, 100, CallType::NoPower, std::chrono::system_clock::now());
    EXPECT_TRUE(call.isOpen());
    EXPECT_EQ(call.getStatus(), CallStatus::Open);
}

TEST(TroubleCallTest, AssignSetsStatusAndOutage) {
    Trouble_Call call(1, 100, CallType::NoPower, std::chrono::system_clock::now());
    call.assignTo(5);
    EXPECT_EQ(call.getStatus(), CallStatus::Assigned);
    EXPECT_EQ(call.getOutageId(), 5);
}

TEST(TroubleCallTest, AssignClosedCall) {
    Trouble_Call call(1, 100, CallType::NoPower, std::chrono::system_clock::now());
    call.close();
    EXPECT_THROW(call.assignTo(5), std::logic_error);
}

TEST(TroubleCallTest, FlickeringDoesntCauseOutage) {
    Trouble_Call call(1, 100, CallType::Flickering, std::chrono::system_clock::now());
    EXPECT_FALSE(call.causesOutage());
}