#pragma once
#include <string>
#include <vector>
#include "service_point.h"

class LV_Feeder{
    public:
        LV_Feeder();
        LV_Feeder(int id, std::string& name);
        LV_Feeder(int id, std::string& name, std::vector<Service_Point>& points);
        LV_Feeder(const LV_Feeder& feeder);

        void setId(int id);
        void setName(std::string& name);
        void setPoints(std::vector<Service_Point>& points);

        int getId() const;
        std::string getName() const;
        std::vector<Service_Point> getPoints() const;
    private:
        int feeder_id;
        std::string feeder_name;
        std::vector<Service_Point> service_points;
};