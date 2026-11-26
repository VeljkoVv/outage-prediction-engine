#include <gtest/gtest.h>
#include <sqlite3.h>
#include <chrono>
#include <stdexcept>
#include "../src/database.h"
#include "../src/network.h"
#include "../src/predictor_engine.h"


class EngineTest : public ::testing::Test {
    protected:
        Network net;

        void SetUp() override {
            sqlite3* db = nullptr;
            ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);

            const char* sql =
                "CREATE TABLE lv_transformer (transformer_id INTEGER PRIMARY KEY, transformer_name TEXT);"
                "CREATE TABLE lv_feeder (feeder_id INTEGER PRIMARY KEY, feeder_name TEXT);"
                "CREATE TABLE service_point (point_id INTEGER PRIMARY KEY, feeder_id INTEGER, point_name TEXT);"
                "CREATE TABLE service_delivery_point (delivery_id INTEGER PRIMARY KEY, point_id INTEGER, delivery_name TEXT, delivery_adress TEXT);"
                "INSERT INTO lv_transformer VALUES (1, 'TS Test');"
                "INSERT INTO lv_feeder VALUES (10, 'Feeder 1'), (20, 'Feeder 2'), (30, 'Feeder 3');"
                "INSERT INTO service_point VALUES"
                "  (100, 10, 'House A'), (101, 10, 'House B'),"
                "  (200, 20, 'House C'), (201, 20, 'House D'),"
                "  (300, 30, 'House E');"
                "INSERT INTO service_delivery_point VALUES"
                "  (1000, 100, 'Customer 1', ''), (1001, 100, 'Customer 2', ''), (1010, 101, 'Customer 3', ''),"
                "  (2000, 200, 'Customer 4', ''), (2010, 201, 'Customer 5', ''),"
                "  (3000, 300, 'Customer 6', '');";
            ASSERT_EQ(sqlite3_exec(db, sql, nullptr, nullptr, nullptr), SQLITE_OK);

            Database d(db);
            net.loadNetwork(d);
        }

        Trouble_Call call(int id, int sdpId, CallType type = CallType::NoPower) {
            return Trouble_Call(id, sdpId, type, std::chrono::system_clock::now());
        }
};

TEST_F(EngineTest, ServicePointOutage) {
    PredictorEngine engine(net);
    int o = engine.onTroubleCall(call(1, 1000));

    EXPECT_TRUE(engine.getOutage(o).getType() == ElementType::ServicePoint);
    EXPECT_EQ(engine.getOutage(o).getElementId(), 100);
    EXPECT_EQ(engine.openOutageCount(), 1);
}

TEST_F(EngineTest, SameServicePoint) {
    PredictorEngine engine(net);
    int o1 = engine.onTroubleCall(call(1, 1000));
    int o2 = engine.onTroubleCall(call(2, 1001));

    EXPECT_EQ(o1, o2);
    EXPECT_EQ(engine.getOutage(o1).getCallIds().size(), 2);
    EXPECT_EQ(engine.openOutageCount(), 1);
}

TEST_F(EngineTest, RollUpToFeeder) {
    PredictorEngine engine(net);
    int o1 = engine.onTroubleCall(call(1, 1000));
    int o2 = engine.onTroubleCall(call(2, 1010));

    const Outage& feederOutage = engine.getOutage(o2);
    EXPECT_TRUE(feederOutage.getType() == ElementType::Feeder);
    EXPECT_EQ(feederOutage.getElementId(), 10);
    EXPECT_EQ(feederOutage.getCallIds().size(), 2);
    EXPECT_TRUE(engine.getOutage(o1).getStatus() == OutageStatus::Merged);
    EXPECT_EQ(engine.openOutageCount(), 1);
}

TEST_F(EngineTest, JoinsFeeder) {
    PredictorEngine engine(net);
    engine.onTroubleCall(call(1, 1000));
    int feederOutage = engine.onTroubleCall(call(2, 1010));
    int o3 = engine.onTroubleCall(call(3, 1001));

    EXPECT_EQ(o3, feederOutage);
    EXPECT_EQ(engine.getOutage(feederOutage).getCallIds().size(), 3);
}

TEST_F(EngineTest, RollUpToTransformer) {
    PredictorEngine engine(net);
    engine.onTroubleCall(call(1, 1000));
    engine.onTroubleCall(call(2, 1010));
    engine.onTroubleCall(call(3, 2000));
    int o = engine.onTroubleCall(call(4, 2010));

    EXPECT_TRUE(engine.getOutage(o).getType() == ElementType::Transformer);
    EXPECT_EQ(engine.getOutage(o).getCallIds().size(), 4);
    EXPECT_EQ(engine.openOutageCount(), 1);
}

TEST_F(EngineTest, FlickeringCreatesNoOutage) {
    PredictorEngine engine(net);
    int o = engine.onTroubleCall(call(1, 1000, CallType::Flickering));

    EXPECT_EQ(o, 0);
    EXPECT_EQ(engine.openOutageCount(), 0);
    EXPECT_EQ(engine.openCallCount(), 1);
}

TEST_F(EngineTest, AlertsDispatch) {
    PredictorEngine engine(net);
    engine.onTroubleCall(call(1, 1000, CallType::Hazard));

    EXPECT_EQ(engine.getEmergencies().size(), 1);
}

TEST_F(EngineTest, RestoreClosesOutageAndCalls) {
    PredictorEngine engine(net);
    int o1 = engine.onTroubleCall(call(1, 1000));
    engine.onTroubleCall(call(2, 1001));
    engine.restoreOutage(o1, std::chrono::system_clock::now());

    EXPECT_EQ(engine.openOutageCount(), 0);
    EXPECT_EQ(engine.openCallCount(), 0);

    int o2 = engine.onTroubleCall(call(3, 1000));
    EXPECT_NE(o2, o1);
}

TEST_F(EngineTest, UnknownDeliveryPoint) {
    PredictorEngine engine(net);

    EXPECT_THROW(engine.onTroubleCall(call(1, 999)), std::out_of_range);
    EXPECT_EQ(engine.openCallCount(), 0);
}

TEST_F(EngineTest, DuplicateId) {
    PredictorEngine engine(net);
    engine.onTroubleCall(call(1, 1000));

    EXPECT_THROW(engine.onTroubleCall(call(1, 1001)), std::logic_error);
}