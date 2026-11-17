#include "service_point.h"
#include "service_delivery_point.h"

Service_Point::Service_Point() {}
Service_Point::Service_Point(int id, std::string& name) : point_id(id), point_name(name) {}
Service_Point::Service_Point(const Service_Point& point) : point_id(point.point_id), point_name(point.point_name) {}

void Service_Point::setId(int id) {point_id = id;}
void Service_Point::setFeederId(int id) {feeder_id = id;}
void Service_Point::setName(const std::string& name) {point_name = name;}

int Service_Point::getId() const {return point_id;}
int Service_Point::getFeederId() const {return feeder_id;}
std::string Service_Point::getName() const {return point_name;}
