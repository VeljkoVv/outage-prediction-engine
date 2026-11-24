#pragma once
#include <unordered_map>
#include <optional>
#include "network.h"
#include "outage.h"
#include "trouble_call.h"

class PredictorEngine {
public:
    PredictorEngine(const Network& net);

    int onTroubleCall(const Trouble_Call& call);
    void tryRollUpFeeder(int id);
    void tryRollUpTransformer();

    int openOutageCount() const;
    int openCallCount() const;

    std::optional<int> findOpenOutage(ElementType type, int elementId) const;
    int createOutage(ElementType type, int elementId, Outage::TimePoint startedAt);

    void alertDispatch(int callId);
    const std::vector<int>& getEmergencies() const;
    const std::unordered_map<int, Outage>& getOutages() const;
    const Outage& getOutage(int id) const;

private:
   
    const Network& network;
    std::unordered_map<int, Trouble_Call> calls;
    std::unordered_map<int, Outage> outages;
    std::vector<int> emergencies;
    int next_outage_id = 1;
};