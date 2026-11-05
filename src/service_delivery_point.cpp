#include "service_delivery_point.h"
#include <iostream>

Service_Delivery_Point::Service_Delivery_Point() : delivery_id(0), delivery_name(""), delivery_adress("") {}
Service_Delivery_Point::Service_Delivery_Point(int id, std::string& name, std::string& adress) : delivery_id(id), delivery_name(name), delivery_adress(adress) {}
Service_Delivery_Point::Service_Delivery_Point(const Service_Delivery_Point& points) :  delivery_id(points.delivery_id), delivery_name(points.delivery_name), delivery_adress(points.delivery_adress) {}

void Service_Delivery_Point::setId(int id) {delivery_id = id;}
void Service_Delivery_Point::setName(std::string& name) {delivery_name = name;}
void Service_Delivery_Point::setAdress(std::string& adress) {delivery_adress = adress;}

int Service_Delivery_Point::getId() const {return delivery_id;}
std::string Service_Delivery_Point::getName() const {return delivery_name;}
std::string Service_Delivery_Point::getAdress() const {return delivery_adress;}