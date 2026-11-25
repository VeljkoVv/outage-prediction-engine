#include "predictor_engine.h"
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

PredictorEngine::PredictorEngine(const Network& net): network(net) {}

void PredictorEngine::loadState(Database& db){
    for(const auto& o : db.loadOutages()){
        addOutage(o);
    }
    for(const auto& tc : db.loadTroubleCalls()){
        addTroubleCall(tc);
    }

    for (const auto& [tcId, tc] : calls) {
        int outageId = tc.getOutageId();
        auto it = outages.find(outageId);
        if (it != outages.end()) {
            it->second.addCall(tcId);
        }
    }

    next_outage_id = db.maxId("outage", "outage_id") + 1;
    next_call_id = db.maxId("trouble_call", "call_id") + 1;
}

void PredictorEngine::saveState(Database& db) {
    db.exec("BEGIN;");
    try {
        for (const auto& [id, outage] : outages) {
            db.saveOutage(outage);
        }
        for (const auto& [id, call] : calls) {
            db.saveCall(call);
        }
        db.exec("COMMIT;");
    } catch (...) {
        db.exec("ROLLBACK;");
        throw;
    }
}

void PredictorEngine::addOutage(const Outage& o){
    outages.insert({o.getId(), o});
}

void PredictorEngine::addTroubleCall(const Trouble_Call& tc){
    calls.insert({tc.getId(), tc});
}

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

    tryRollUpFeeder(feederId);

    return stored.getOutageId();
}

void PredictorEngine::tryRollUpFeeder(int id){
    std::vector<int> spOutages;
    for(const auto& a : network.servicePointsOf(id)){
        std::optional<int> outageId = findOpenOutage(ElementType::ServicePoint, a);
        if (outageId && !outages.at(*outageId).isConfirmed()) {
            spOutages.push_back(*outageId);
        }
    }
    if(spOutages.size() < 2){
        return;
    }
    
    Outage::TimePoint start = outages.at(spOutages.at(0)).getStartedAt();
    for (int spOutageId : spOutages) {
        Outage::TimePoint t = outages.at(spOutageId).getStartedAt();
        if (t < start) {
            start = t;
        }
    }

    int feederOutageId = createOutage(ElementType::Feeder, id, start);
    Outage& feederOutage = outages.at(feederOutageId);

    for (int spOutageId : spOutages) {
        Outage& spOutage = outages.at(spOutageId);

        feederOutage.takeCallsFrom(spOutage);
        for (int callId : spOutage.getCallIds()) {
            calls.at(callId).assignTo(feederOutageId);
        }
        spOutage.mergeInto(feederOutageId);
    }

    tryRollUpTransformer();
}

void PredictorEngine::tryRollUpTransformer(){
    std::vector<int> feederOutages;
    for(const auto& [feederId, feeder] : network.getFeeders()){
        std::optional<int> outageId = findOpenOutage(ElementType::Feeder, feederId);
        if (outageId && !outages.at(*outageId).isConfirmed()) {
            feederOutages.push_back(*outageId);
        }
    }
    if(feederOutages.size() < 2){
        return;
    }
    
    Outage::TimePoint start = outages.at(feederOutages.at(0)).getStartedAt();
    for (int feederOutageId : feederOutages) {
        Outage::TimePoint t = outages.at(feederOutageId).getStartedAt();
        if (t < start) {
            start = t;
        }
    }

    int transformerOutageId = createOutage(ElementType::Transformer, network.getTransformer().getId(), start);
    Outage& transformerOutage = outages.at(transformerOutageId);

    for (int feederOutageId : feederOutages) {
        Outage& feederOutage = outages.at(feederOutageId);

        transformerOutage.takeCallsFrom(feederOutage);
        for (int callId : feederOutage.getCallIds()) {
            calls.at(callId).assignTo(transformerOutageId);
        }
        feederOutage.mergeInto(transformerOutageId);
    }
}

int PredictorEngine::openOutageCount() const {
    int count = 0;
    for (const auto& [id, outage] : outages) {
        if (outage.isOpen()) {
            count++;
        }
    }
    return count;
}

int PredictorEngine::predictedOutageCount() const {
    int count = 0;
    for (const auto& [id, outage] : outages) {
        if (outage.isOpen() && outage.getStatus() == OutageStatus::Predicted) {
            count++;
        }
    }
    return count;
}

int PredictorEngine::openCallCount() const {
    int count = 0;
    for (const auto& [id, call] : calls) {
        if (call.isOpen()) {
            count++;
        }
    }
    return count;
}

void PredictorEngine::confirmOutage(int id) {
    auto it = outages.find(id);
    if (it == outages.end()) {
        throw std::out_of_range("Outage " + std::to_string(id) + " not found");
    }
    it->second.confirm();
}

void PredictorEngine::restoreOutage(int id, Outage::TimePoint at) {
    auto it = outages.find(id);
    if (it == outages.end()) {
        throw std::out_of_range("Outage " + std::to_string(id) + " not found");
    }
    it->second.restore(at);

    for (int callId : it->second.getCallIds()) {
        calls.at(callId).close();
    }
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

const std::unordered_map<int, Outage>& PredictorEngine::getOutages() const{
    return outages;
}

const std::unordered_map<int, Trouble_Call>& PredictorEngine::getCalls() const{
    return calls;
}

const Outage& PredictorEngine::getOutage(int id) const{
    return outages.at(id);
}

const Trouble_Call& PredictorEngine::getCall(int id) const {
    return calls.at(id);
}

int PredictorEngine::newCallId() {
    return next_call_id++;
}