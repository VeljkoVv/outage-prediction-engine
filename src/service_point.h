#pragma once
#include <string>
#include <vector>
#include "service_delivery_point.h"

class Service_Point{
    public:
        Service_Point();
        Service_Point(int id, std::string& name);
        Service_Point(const Service_Point& point);

        void setId(int id);
        void setFeederId(int id);
        void setName(const std::string& name);

        int getId() const;
        int getFeederId() const;
        std::string getName() const;

    private:
        int point_id;
        int feeder_id;
        std::string point_name;
};