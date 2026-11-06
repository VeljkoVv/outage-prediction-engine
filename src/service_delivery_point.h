#pragma once
#include <string>
#include <vector>


class Service_Delivery_Point{
    public:
        Service_Delivery_Point();
        Service_Delivery_Point(int id, std::string& name, std::string& adress);
        Service_Delivery_Point(const Service_Delivery_Point& points);

        void setId(int id);
        void setName(std::string& name);
        void setAdress(std::string& adress);

        int getId() const;
        std::string getName() const;
        std::string getAdress() const;
    private:
        int delivery_id;
        std::string delivery_name;
        std::string delivery_adress;
};