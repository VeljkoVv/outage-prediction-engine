#pragma once
#include <string>
#include <vector>
#include "lv_feeder.h"

class LV_Transformer{
    public:
        LV_Transformer();
        LV_Transformer(int id, std::string& name);       

        void setId(int id);
        void setName(const std::string& name);

        int getId() const;
        std::string getName() const;

    private:
        int transformer_id;
        std::string transformer_name;
};