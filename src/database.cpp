#include "database.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

Database::Database(const std::string& path) : db_p(nullptr){
    if(sqlite3_open_v2(path.c_str(), &db_p, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK){
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_close(db_p);
        throw std::runtime_error("\nError opening DB connection: " + err);
    }

    if (sqlite3_exec(db_p, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_close(db_p);
        throw std::runtime_error("Error enabling foreign keys: " + err);
    }
}
Database::Database(sqlite3* db) : db_p(db) {}

Database::~Database(){
    if (db_p == nullptr) {
        return;
    }
    
    int status = sqlite3_close_v2(db_p);

    if (status == SQLITE_OK) {
        std::cerr << "\nDatabase succesfully closed.\n";
    }
    else {
    std::cerr << "\nError closing the Database (" << status << "): " << sqlite3_errstr(status) << '\n';
    }
    db_p = nullptr;
}

LV_Transformer Database::loadTransformer(){
    LV_Transformer transformer;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT transformer_id, transformer_name FROM lv_transformer;";

    if (sqlite3_prepare_v2(db_p, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        throw std::runtime_error("Error on query: " + err);
    }
    int count = 0;
    int rc;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        transformer.setId(sqlite3_column_int(stmt, 0));
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        transformer.setName(name ? reinterpret_cast<const char*>(name) : "");
        count++;
    }

    if (rc != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error reading rows: " + err);
    }

    sqlite3_finalize(stmt);
    if(count != 1){
        throw std::runtime_error("Expected exactly one transformer, found " + std::to_string(count));
    }
    
    return transformer;
}
std::vector<LV_Feeder> Database::loadFeeders(){
    std::vector<LV_Feeder> feeders;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT feeder_id, feeder_name FROM lv_feeder;";

    if (sqlite3_prepare_v2(db_p, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        throw std::runtime_error("Error on query: " + err);
    }

    int rc;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        LV_Feeder f;
        f.setId(sqlite3_column_int(stmt, 0));
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        f.setName(name ? reinterpret_cast<const char*>(name) : "");
        feeders.push_back(f);
    }

    if (rc != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error reading rows: " + err);
    }
    sqlite3_finalize(stmt);
    return feeders;
}
std::vector<Service_Point> Database::loadServicePoints(){
    std::vector<Service_Point> points;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT point_id, feeder_id, point_name FROM service_point;";

    if (sqlite3_prepare_v2(db_p, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        throw std::runtime_error("Error on query: " + err);
    }

    int rc;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        Service_Point sp;
        sp.setId(sqlite3_column_int(stmt, 0));
        sp.setFeederId(sqlite3_column_int(stmt, 1));
        const unsigned char* name = sqlite3_column_text(stmt, 2);
        sp.setName(name ? reinterpret_cast<const char*>(name) : "");
        points.push_back(sp);
    }

    if (rc != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error reading rows: " + err);
    }
    sqlite3_finalize(stmt);
    return points;
}
std::vector<Service_Delivery_Point> Database::loadServiceDeliveryPoints(){
    std::vector<Service_Delivery_Point> dpoints;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT delivery_id, point_id, delivery_name FROM service_delivery_point;";

    if (sqlite3_prepare_v2(db_p, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        throw std::runtime_error("Error on query: " + err);
    }

    int rc;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        Service_Delivery_Point sdp;
        sdp.setId(sqlite3_column_int(stmt, 0));
        sdp.setPointId(sqlite3_column_int(stmt, 1));
        const unsigned char* name = sqlite3_column_text(stmt, 2);
        sdp.setName(name ? reinterpret_cast<const char*>(name) : "");
        dpoints.push_back(sdp);
    }

    if (rc != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error reading rows: " + err);
    }
    sqlite3_finalize(stmt);
    return dpoints;
}

std::vector<Outage> Database::loadOutages(){
    std::vector<Outage> outages;
    sqlite3_stmt* stmt = nullptr;

    const char* sql = "SELECT outage_id, type, service_point_id, feeder_id, transformer_id, status, started_at, ended_at, merged_id FROM outage;";

    if (sqlite3_prepare_v2(db_p, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_p);
        throw std::runtime_error("Error on query: " + err);
    }

    int rc;
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        Outage outage;
        outage.setId(sqlite3_column_int(stmt, 0));

        const unsigned char* typeText = sqlite3_column_text(stmt, 1);
        std::string typeStr = typeText ? reinterpret_cast<const char*>(typeText) : "";
        ElementType type = toElementType(typeStr);
        outage.setType(type);
        
        int sp_id = sqlite3_column_int(stmt, 2);
        int feeder_id = sqlite3_column_int(stmt, 3);
        int transformer_id = sqlite3_column_int(stmt, 4);
        if(sp_id){
            outage.setElementId(sp_id);
        } else if(feeder_id){
            outage.setElementId(feeder_id);
        } else{
            outage.setElementId(transformer_id);
        }

        const unsigned char* statusText = sqlite3_column_text(stmt, 5);
        std::string statusStr = statusText ? reinterpret_cast<const char*>(statusText) : "";
        OutageStatus status = toStatus(statusStr);
        outage.setStatus(status);

        std::time_t secsS = static_cast<std::time_t>(sqlite3_column_int64(stmt, 6));
        Outage::TimePoint startedAt = std::chrono::system_clock::from_time_t(secsS);
        outage.setStartedAt(startedAt);

        if (sqlite3_column_type(stmt, 7) != SQLITE_NULL){
            std::time_t secsE = static_cast<std::time_t>(sqlite3_column_int64(stmt, 7));
            Outage::TimePoint endedAt = std::chrono::system_clock::from_time_t(secsE);
            outage.setEndedAt(endedAt);
        }

        if (int id = sqlite3_column_type(stmt, 8) != SQLITE_NULL){
            outage.setMergedInto(id);
        }

        outages.push_back(outage);
    }

    if (rc != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_p);
        sqlite3_finalize(stmt);
        throw std::runtime_error("Error reading rows: " + err);
    }
    sqlite3_finalize(stmt);
    return outages;
}

ElementType Database::toElementType(const std::string& text) {
    if (text == "ServicePoint") {
        return ElementType::ServicePoint;
    }
    if (text == "Feeder") {
        return ElementType::Feeder;
    }
    if (text == "Transformer") {
        return ElementType::Transformer;
    }
    throw std::runtime_error("Unknown element type: " + text);
}

OutageStatus Database::toStatus(const std::string& text) {
    if (text == "Predicted") {
        return OutageStatus::Predicted;
    }
    if (text == "Confirmed") {
        return OutageStatus::Confirmed;
    }
    if (text == "Restored") {
        return OutageStatus::Restored;
    }
    if (text == "Merged") {
        return OutageStatus::Merged;
    }
    else {
        return OutageStatus::Cancelled;
    }
    throw std::runtime_error("Unknown status: " + text);
}