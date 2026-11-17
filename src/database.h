#pragma once
#include "sqlite3.h"
#include "lv_transformer.h"
#include <string>

class Database{
    public:
        Database(const std::string& path);
        Database(sqlite3* db);
        ~Database();
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

        LV_Transformer loadTransformer();
        std::vector<LV_Feeder> loadFeeders();
        std::vector<Service_Point> loadServicePoints();
        std::vector<Service_Delivery_Point> loadServiceDeliveryPoints();
    private:
        sqlite3* db_p = nullptr;
};