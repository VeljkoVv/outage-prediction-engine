#pragma once
#include "sqlite3.h"
#include <string>

class Database{
    public:
        Database(const std::string& path);
        Database(sqlite3* db);

        void LoadElements();

    private:
        sqlite3* db_p = nullptr;
};