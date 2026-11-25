#pragma once
#include "sqlite3.h"
#include "lv_transformer.h"
#include "outage.h"
#include "trouble_call.h"
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
        std::vector<Outage> loadOutages();
        std::vector<Trouble_Call> loadTroubleCalls();

        ElementType toElementType(const std::string& text);
        OutageStatus toStatus(const std::string& text);
        CallType toCallType(const std::string& text);
        CallStatus toCallStatus(const std::string& text);

        int maxId(const std::string& table, const std::string& column);
    private:
        sqlite3* db_p = nullptr;
};