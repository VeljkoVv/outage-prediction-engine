#include "network.h"
#include <stdexcept>


void Network::loadNetwork(Database& db){
    setTransformer(db.loadTransformer());

    for(const auto& f : db.loadFeeders()){
        addFeeder(f);
    }
    for(const auto& sp : db.loadServicePoints()){
        addServicePoint(sp);
    }
    for(const auto& sdp : db.loadServiceDeliveryPoints()){
        addSdp(sdp);
    }
}

void Network::buildIndexes() {
    points_of_feeder.clear();
    sdps_of_point.clear();

    for (const auto& [id, sp] : service_points) {
        points_of_feeder[sp.getFeederId()].push_back(id);
    }
    for (const auto& [id, sdp] : delivery_points) {
        sdps_of_point[sdp.getPointId()].push_back(id);
    }
}

const std::vector<int>& Network::servicePointsOf(int feederId) const{
    static const std::vector<int> empty;
    auto it = points_of_feeder.find(feederId);
    if (it == points_of_feeder.end()) {
        return empty;
    }
    return it->second;
}

const std::vector<int>& Network::sdpsOf(int servicePointId) const{
    static const std::vector<int> empty;
    auto it = sdps_of_point.find(servicePointId);
    if (it == sdps_of_point.end()) {
        return empty;
    }
    return it->second;
}

void Network::setTransformer(const LV_Transformer& t){
    transformer = t;
}      
void Network::addFeeder(const LV_Feeder& f){
    feeders.insert({f.getId(), f});
}
void Network::addServicePoint(const Service_Point& sp){
    service_points.insert({sp.getId(), sp});
}
void Network::addSdp(const Service_Delivery_Point& sdp){
    delivery_points.insert({sdp.getId(), sdp});
}

const LV_Transformer& Network::getTransformer() const{
    return transformer;
}
const LV_Feeder& Network::getFeeder(int id) const{
    auto it = feeders.find(id);
    if (it == feeders.end()) {
        throw std::out_of_range("Feeder " + std::to_string(id) + " not found in network");
    }
    return it->second;
}
const Service_Point& Network::getServicePoint(int id) const{
    auto it = service_points.find(id);
    if (it == service_points.end()) {
        throw std::out_of_range("Service Point " + std::to_string(id) + " not found in network");
    }
    return it->second;
}
const Service_Delivery_Point& Network::getSdp(int id) const{
    auto it = delivery_points.find(id);
    if (it == delivery_points.end()) {
        throw std::out_of_range("Service Delivery Point " + std::to_string(id) + " not found in network");
    }
    return it->second;
}