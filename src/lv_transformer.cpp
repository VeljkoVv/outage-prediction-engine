#include "lv_transformer.h"
#include "lv_feeder.h"

LV_Transformer::LV_Transformer() {}
LV_Transformer::LV_Transformer(int id, std::string& name) : transformer_id(id), transformer_name(name) {}
        
void LV_Transformer::setId(int id) {transformer_id = id;}
void LV_Transformer::setName(const std::string& name) {transformer_name = name;}

int LV_Transformer::getId() const {return transformer_id;}
std::string LV_Transformer::getName() const {return transformer_name;}
