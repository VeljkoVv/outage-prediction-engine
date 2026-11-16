#pragma once
#include <string>
#include <vector>
#include "service_point.h"

class LV_Feeder{
    public:
        LV_Feeder();
        LV_Feeder(int id, std::string& name);
        LV_Feeder(const LV_Feeder& feeder);

        void setId(int id);
        void setName(const std::string& name);

        int getId() const;
        std::string getName() const;
    private:
        int feeder_id;
        std::string feeder_name;
};