#include "service_point.h"
#include "service_delivery_point.h"

Service_Point::Service_Point() {}
Service_Point::Service_Point(int id, std::string& name) : point_id(id), point_name(name) {}
Service_Point::Service_Point(int id, std::string& name, std::vector<Service_Delivery_Point>& points) : point_id(id), point_name(name), delivery_points(points) {}
Service_Point::Service_Point(const Service_Point& point) : point_id(point.point_id), point_name(point.point_name), delivery_points(point.delivery_points) {}

void Service_Point::setId(int id) {point_id = id;}
void Service_Point::setName(std::string& name) {point_name = name;}
void Service_Point::setDeliveryPoints(std::vector<Service_Delivery_Point>& points) {delivery_points = points;}

int Service_Point::getId() const {return point_id;}
std::string Service_Point::getName() const {return point_name;}
std::vector<Service_Delivery_Point> Service_Point::getDeliveryPoints() const {return delivery_points;}
