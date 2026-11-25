#include <gtest/gtest.h>
#include <sqlite3.h>
#include <sstream>
#include "../src/database.h"
#include "../src/network.h"
#include "../src/predictor_engine.h"
#include "../src/menu.h"

class MenuTest : public ::testing::Test {
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
                "INSERT INTO lv_feeder VALUES (10, 'Feeder 1');"
                "INSERT INTO service_point VALUES (100, 10, 'House A');"
                "INSERT INTO service_delivery_point VALUES (1000, 100, 'Customer 1', '');";
            ASSERT_EQ(sqlite3_exec(db, sql, nullptr, nullptr, nullptr), SQLITE_OK);

            Database d(db);
            net.loadNetwork(d);
        }

        std::string run(PredictorEngine& engine, const std::string& typed) {
            std::istringstream in(typed);
            std::ostringstream out;
            Database d(":memory:");
            Menu menu(engine, net, d, in, out);
            menu.runMenu();
            return out.str();
        }
};

TEST_F(MenuTest, NewCall) {
    PredictorEngine engine(net);
    run(engine, "2\n1000\n1\n0\n");

    EXPECT_EQ(engine.openOutageCount(), 1);
}

TEST_F(MenuTest, UnknownPoint) {
    PredictorEngine engine(net);
    std::string output = run(engine, "2\n999\n1\n0\n");

    EXPECT_NE(output.find("Error:"), std::string::npos);
    EXPECT_EQ(engine.openCallCount(), 0);
}

TEST_F(MenuTest, ConfirmOutage) {
    PredictorEngine engine(net);
    run(engine, "2\n1000\n1\n4\n1\n0\n");

    EXPECT_TRUE(engine.getOutage(1).isConfirmed());
}

TEST_F(MenuTest, RestorePower) {
    PredictorEngine engine(net);
    run(engine, "2\n1000\n1\n5\n1\n0\n");

    EXPECT_EQ(engine.openOutageCount(), 0);
    EXPECT_EQ(engine.openCallCount(), 0);
}