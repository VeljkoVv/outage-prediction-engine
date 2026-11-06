#include "trouble_call.h"

    Trouble_Call::Trouble_Call()
    : id(0),
      delivery_point_id(0),
      type(CallType::NoPower),
      received_at(std::chrono::system_clock::now()),
      status(CallStatus::Open){}

Trouble_Call::Trouble_Call(int id, int dp_id, CallType t, TimePoint tp, CallStatus s)
    : id(id),
      delivery_point_id(dp_id),
      type(t),
      received_at(tp),
      status(s) {}

    int Trouble_Call::getId() const { return id; }
    int Trouble_Call::getDeliveryPointId() const { return delivery_point_id; }
    CallType Trouble_Call::getType() const { return type; }
    Trouble_Call::TimePoint Trouble_Call::getReceivedAt() const { return received_at; }
    CallStatus Trouble_Call::getStatus() const { return status; }

    void Trouble_Call::setDeliveryPointId(int value) { delivery_point_id = value; }
    void Trouble_Call::setType(CallType value) { type = value; }
    void Trouble_Call::setReceivedAt(TimePoint value) { received_at = value; }
    void Trouble_Call::setStatus(CallStatus value) { status = value; }