#include "network.h"

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
    return feeders.at(id);
}
const Service_Point& Network::getServicePoint(int id) const{
    return service_points.at(id);
}
const Service_Delivery_Point& Network::getSdp(int id) const{
    return delivery_points.at(id);
}