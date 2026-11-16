#pragma once
#include "lv_feeder.h"
#include "lv_transformer.h"
#include "service_point.h"
#include "service_delivery_point.h"
#include "database.h"
#include <unordered_map>
#include <vector>



class Network{
    public:
        void setTransformer(const LV_Transformer& t);       
        void addFeeder(const LV_Feeder& f);
        void addServicePoint(const Service_Point& sp);
        void addSdp(const Service_Delivery_Point& sdp);

        const LV_Transformer& getTransformer() const;
        const LV_Feeder& getFeeder(int id) const;
        const Service_Point& getServicePoint(int id) const;
        const Service_Delivery_Point& getSdp(int id) const;

        void loadNetwork(Database& db);
        void buildIndexes();

    private:
        LV_Transformer transformer;
        std::unordered_map<int, LV_Feeder> feeders;
        std::unordered_map<int, Service_Point> service_points;
        std::unordered_map<int, Service_Delivery_Point> delivery_points;

        std::unordered_map<int, std::vector<int>> points_of_feeder;
        std::unordered_map<int, std::vector<int>> sdps_of_point;
};