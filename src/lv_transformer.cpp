#include "lv_transformer.h"
#include "lv_feeder.h"

LV_Transformer::LV_Transformer() {}
LV_Transformer::LV_Transformer(int id, std::string& name) : transformer_id(id), transformer_name(name) {}
LV_Transformer::LV_Transformer(int id, std::string& name, std::vector<LV_Feeder>& feeders) : transformer_id(id), transformer_name(name), feeders(feeders) {}
LV_Transformer::LV_Transformer(const LV_Transformer& transformer) : transformer_id(transformer.transformer_id), transformer_name(transformer.transformer_name), feeders(transformer.feeders) {}
        
void LV_Transformer::setId(int id) {transformer_id = id;}
void LV_Transformer::setName(std::string& name) {transformer_name = name;}
void LV_Transformer::setFeeders(std::vector<LV_Feeder>& feeders) {feeders = feeders;}

int LV_Transformer::getId() const {return transformer_id;}
std::string LV_Transformer::getName() const {return transformer_name;}
std::vector<LV_Feeder> LV_Transformer::getFeeders() const {return feeders;}