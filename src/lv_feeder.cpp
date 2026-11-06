#include "lv_feeder.h"
#include "service_point.h"

LV_Feeder::LV_Feeder(){}
LV_Feeder::LV_Feeder(int id, std::string& name) : feeder_id(id), feeder_name(name) {}
LV_Feeder::LV_Feeder(int id, std::string& name, std::vector<Service_Point>& points) : feeder_id(id), feeder_name(name), service_points(points) {}
LV_Feeder::LV_Feeder(const LV_Feeder& feeder) : feeder_id(feeder.feeder_id), feeder_name(feeder.feeder_name), service_points(feeder.service_points) {}

void LV_Feeder::setId(int id) {feeder_id = id;}
void LV_Feeder::setName(std::string& name) {feeder_name = name;}
void LV_Feeder::setPoints(std::vector<Service_Point>& points) {service_points = points;}

int LV_Feeder::getId() const {return feeder_id;}
std::string LV_Feeder::getName() const {return feeder_name;}
std::vector<Service_Point> LV_Feeder::getPoints() const {return service_points;}