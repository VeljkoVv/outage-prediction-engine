#include <gtest/gtest.h>
#include <sqlite3.h>
#include <vector>
#include "../src/database.h"
#include "../src/network.h"

class NetworkTest : public ::testing::Test {
    protected:
        sqlite3* db = nullptr;

        void SetUp() override {
            ASSERT_EQ(sqlite3_open(":memory:", &db), SQLITE_OK);

            const char* sql =
                "CREATE TABLE lv_transformer (transformer_id INTEGER PRIMARY KEY, transformer_name TEXT);"
                "CREATE TABLE lv_feeder (feeder_id INTEGER PRIMARY KEY, feeder_name TEXT);"
                "CREATE TABLE service_point (point_id INTEGER PRIMARY KEY, feeder_id INTEGER, point_name TEXT);"
                "CREATE TABLE service_delivery_point (delivery_id INTEGER PRIMARY KEY, point_id INTEGER, delivery_name TEXT, delivery_adress);"
                "INSERT INTO lv_transformer VALUES (1, 'TS Detelinara');"
                "INSERT INTO lv_feeder VALUES (10, 'Feeder 1'), (20, 'Feeder 2'), (30, 'Feeder 3');"
                "INSERT INTO service_point VALUES"
                "  (100, 10, 'House A'), (101, 10, 'Building B'), (200, 20, 'Building C');"
                "INSERT INTO service_delivery_point VALUES"
                "  (1000, 100, 'Stan 1', 'Janka Veselinovica 5'), (1001, 100, 'Stan 2', 'Janka Veselinovica 5'), (1002, 101, 'Stan 3', 'Janka Cmelika 40');";
            ASSERT_EQ(sqlite3_exec(db, sql, nullptr, nullptr, nullptr), SQLITE_OK);
        }
};

TEST_F(NetworkTest, NoDatabase) {
    sqlite3_close(db);
    EXPECT_THROW(Database("no.db"), std::runtime_error);
}

TEST_F(NetworkTest, LoadingTransformer) {
    Database d(db);
    Network net;
    net.loadNetwork(d);

    EXPECT_EQ(net.getTransformer().getId(), 1);
    EXPECT_EQ(net.getTransformer().getName(), "TS Detelinara");
}

TEST_F(NetworkTest, LoadingElements) {
    Database d(db);
    Network net;
    net.loadNetwork(d);

    EXPECT_EQ(net.getFeeder(20).getName(), "Feeder 2");
    EXPECT_EQ(net.getServicePoint(101).getFeederId(), 10);
    EXPECT_EQ(net.getSdp(1002).getPointId(), 101);
}

TEST_F(NetworkTest, doesntExist) {
    Database d(db);
    Network net;
    net.loadNetwork(d);

    EXPECT_THROW(net.getSdp(999), std::out_of_range);
}

TEST_F(NetworkTest, Empty) {
    Database d(db);
    Network net;
    net.loadNetwork(d);
    net.buildIndexes();

    EXPECT_TRUE(net.servicePointsOf(30).empty());
    EXPECT_TRUE(net.sdpsOf(200).empty());
}

TEST_F(NetworkTest, BuildIndexesTwice) {
    Database d(db);
    Network net;
    net.loadNetwork(d);
    net.buildIndexes();
    net.buildIndexes();

    EXPECT_EQ(net.servicePointsOf(10).size(), 2);
}

TEST_F(NetworkTest, TwoTransformators) {
    ASSERT_EQ(sqlite3_exec(db, "INSERT INTO lv_transformer VALUES (2, 'TS Liman');",
                           nullptr, nullptr, nullptr), SQLITE_OK);
    Database d(db);
    Network net;

    EXPECT_THROW(net.loadNetwork(d), std::runtime_error);
}