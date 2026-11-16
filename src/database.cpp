#include "database.h"
#include <stdexcept>

    Database::Database(const std::string& path){
        if(sqlite3_open_v2(path.c_str(), &db_p, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK){
            std::string err = sqlite3_errmsg(db_p);
            sqlite3_close(db_p);
            throw std::runtime_error("\nError opening DB connection: " + err);
        }
    }

    Database::Database(sqlite3* db) : db_p(db) {}