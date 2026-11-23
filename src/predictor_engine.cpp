#include "predictor_engine.h"
#include <iostream>
    
PredictorEngine::PredictorEngine(const Network& net): network(net) {}

int PredictorEngine::onTroubleCall(const Trouble_Call& call){
    int spId = network.getSdp(call.getDeliveryPointId()).getPointId();
    int feederId = network.getServicePoint(spId).getFeederId();
    
    auto [it, inserted] = calls.insert({call.getId(), call});
    if (!inserted) {
        throw std::logic_error("Call " + std::to_string(call.getId()) + " already exists");
    }

    Trouble_Call& stored = it->second;

    if(stored.isEmergency()){
        alertDispatch(call.getId());
    } 
    
    if(!stored.causesOutage()){
        return 0;
    }
    
    int transformerId = network.getTransformer().getId();

    std::optional<int> outageId = findOpenOutage(ElementType::Transformer, transformerId);
    if (!outageId) {
        outageId = findOpenOutage(ElementType::Feeder, feederId);
    }
    if (!outageId) {
        outageId = findOpenOutage(ElementType::ServicePoint, spId);
    }

    if (outageId) {
        outages.at(*outageId).addCall(stored.getId());
        stored.assignTo(*outageId);
        return *outageId;
    }

    int newId = createOutage(ElementType::ServicePoint, spId, stored.getReceivedAt());
    outages.at(newId).addCall(stored.getId());
    stored.assignTo(newId);
    return newId;
}

std::optional<int> PredictorEngine::findOpenOutage(ElementType type, int elementId) const {
    for (const auto& [id, outage] : outages) {
        if (outage.isOpen() && outage.getType() == type && outage.getElementId() == elementId) {
            return id;
        }
    }
    return std::nullopt;
}

int PredictorEngine::createOutage(ElementType type, int elementId, Outage::TimePoint startedAt) {
    int id = next_outage_id++;
    outages.insert({id, Outage(id, type, elementId, startedAt)});
    return id;
}

void PredictorEngine::alertDispatch(int callId) {
    emergencies.push_back(callId);
}

const std::vector<int>& PredictorEngine::getEmergencies() const {
    return emergencies;
}
