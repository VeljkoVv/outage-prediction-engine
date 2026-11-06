#pragma once
#include <string>
#include <vector>
#include "service_delivery_point.h"

class Service_Point{
    public:
        Service_Point();
        Service_Point(int id, std::string& name);
        Service_Point(int id, std::string& name, std::vector<Service_Delivery_Point>& points);
        Service_Point(const Service_Point& point);

        void setId(int id);
        void setName(std::string& name);
        void setDeliveryPoints(std::vector<Service_Delivery_Point>& points);

        int getId() const;
        std::string getName() const;
        std::vector<Service_Delivery_Point> getDeliveryPoints() const;

    private:
        int point_id;
        std::string point_name;
        std::vector<Service_Delivery_Point> delivery_points;
};