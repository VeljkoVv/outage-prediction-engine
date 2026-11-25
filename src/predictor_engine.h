#pragma once
#include <unordered_map>
#include <optional>
#include "network.h"
#include "outage.h"
#include "trouble_call.h"

class PredictorEngine {
public:
    PredictorEngine(const Network& net);

    void loadState(Database& db);

    int onTroubleCall(const Trouble_Call& call);
    void tryRollUpFeeder(int id);
    void tryRollUpTransformer();

    int openOutageCount() const;
    int predictedOutageCount() const;
    int openCallCount() const;
    void confirmOutage(int id);
    void restoreOutage(int id, Outage::TimePoint at);

    std::optional<int> findOpenOutage(ElementType type, int elementId) const;
    int createOutage(ElementType type, int elementId, Outage::TimePoint startedAt);

    void alertDispatch(int callId);
    const std::vector<int>& getEmergencies() const;
    const std::unordered_map<int, Outage>& getOutages() const;
    const std::unordered_map<int, Trouble_Call>& getCalls() const;
    const Outage& getOutage(int id) const;
    const Trouble_Call& getCall(int id) const;

    void addOutage(const Outage& o);
    void addTroubleCall(const Trouble_Call& tc);
    int newCallId();

private:
   
    const Network& network;
    std::unordered_map<int, Trouble_Call> calls;
    std::unordered_map<int, Outage> outages;
    std::vector<int> emergencies;
    int next_outage_id = 1;
    int next_call_id = 1;
};