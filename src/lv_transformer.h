#pragma once
#include <string>
#include <vector>
#include "lv_feeder.h"

class LV_Transformer{
    public:
        LV_Transformer();
        LV_Transformer(int id, std::string& name);
        LV_Transformer(int id, std::string& name, std::vector<LV_Feeder>& feeders);
        LV_Transformer(const LV_Transformer& transformer);
        

        void setId(int id);
        void setName(std::string& name);
        void setFeeders(std::vector<LV_Feeder>& feeders);

        int getId() const;
        std::string getName() const;
        std::vector<LV_Feeder> getFeeders() const;

    private:
        int transformer_id;
        std::string transformer_name;
        std::vector<LV_Feeder> feeders;
};