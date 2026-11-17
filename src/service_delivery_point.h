#pragma once
#include <string>
#include <vector>


class Service_Delivery_Point{
    public:
        Service_Delivery_Point();
        Service_Delivery_Point(int id, std::string& name, std::string& adress);
        Service_Delivery_Point(const Service_Delivery_Point& points);

        void setId(int id);
        void setPointId(int id);
        void setName(const std::string& name);
        void setAdress(const std::string& adress);

        int getId() const;
        int getPointId() const;
        std::string getName() const;
        std::string getAdress() const;
    private:
        int delivery_id;
        int point_id;
        std::string delivery_name;
        std::string delivery_adress;
};