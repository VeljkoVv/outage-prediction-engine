#pragma once
#include <chrono>

enum class CallType {
    NoPower,
    PartialPower,
    Flickering,
    Hazard
};

enum class CallStatus {
    Open,
    Assigned,
    Closed,
    Cancelled
};

class Trouble_Call{
    public:
        using TimePoint = std::chrono::system_clock::time_point;

        Trouble_Call();
        Trouble_Call(int id, int dp_id, CallType t,TimePoint tp, CallStatus s);
        
        void assignTo(int outageId);
        void close();
        void cancel();

        bool isOpen() const;
        bool causesOutage() const;
        bool isEmergency() const;

        int getId() const;
        int getOutageId() const;
        int getDeliveryPointId() const;
        CallType getType() const;
        TimePoint getReceivedAt() const;
        CallStatus getStatus() const;

        void setDeliveryPointId(int value);
        void setOutageId(int value);
        void setType(CallType value);
        void setReceivedAt(TimePoint value);
        void setStatus(CallStatus value);

    private:
        int	id;
        int outage_id;
        int delivery_point_id;
        CallType type;
        TimePoint received_at;
        CallStatus status;
};