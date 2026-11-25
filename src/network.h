#pragma once
#include "lv_feeder.h"
#include "lv_transformer.h"
#include "service_point.h"
#include "service_delivery_point.h"
#include "database.h"
#include "outage.h"
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
        const std::unordered_map<int, LV_Feeder>& getFeeders() const;

        int feederCount() const;
        int servicePointCount() const;
        int sdpCount() const;
        int customersUnder(const Outage& outage) const;

        void loadNetwork(Database& db);
        void buildIndexes();
        const std::vector<int>& servicePointsOf(int feederId) const;
        const std::vector<int>& sdpsOf(int servicePointId) const;

    private:
        LV_Transformer transformer;
        std::unordered_map<int, LV_Feeder> feeders;
        std::unordered_map<int, Service_Point> service_points;
        std::unordered_map<int, Service_Delivery_Point> delivery_points;

        std::unordered_map<int, std::vector<int>> points_of_feeder;
        std::unordered_map<int, std::vector<int>> sdps_of_point;


};