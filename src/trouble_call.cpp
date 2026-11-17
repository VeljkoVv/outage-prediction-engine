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

    void Trouble_Call::assignTo(int outageId){
      if(!isOpen()){
        throw std::logic_error("Cannot assign a closed or cancelled call");
      }
      outage_id = outageId;
      status = CallStatus::Assigned;
    }

    void Trouble_Call::close(){
      if(!isOpen()) {
        throw std::logic_error("Call is already closed or cancelled");
      }
      status = CallStatus::Closed;
    }
    
    void Trouble_Call::cancel(){
      if (!isOpen()) {
        throw std::logic_error("Call is already closed or cancelled");
      }
      status = CallStatus::Cancelled;
    }

    bool Trouble_Call::isOpen() const{
      return status == CallStatus::Open || status == CallStatus::Assigned;
    }

    bool Trouble_Call::causesOutage() const{
      return type == CallType::NoPower || type == CallType::PartialPower;
    }

    bool Trouble_Call::isEmergency() const{
      return type == CallType::Hazard;
    }

    int Trouble_Call::getId() const { return id; }
    int Trouble_Call::getOutageId() const { return outage_id; }
    int Trouble_Call::getDeliveryPointId() const { return delivery_point_id; }
    CallType Trouble_Call::getType() const { return type; }
    Trouble_Call::TimePoint Trouble_Call::getReceivedAt() const { return received_at; }
    CallStatus Trouble_Call::getStatus() const { return status; }

    void Trouble_Call::setDeliveryPointId(int value) { delivery_point_id = value; }
    void Trouble_Call::setOutageId(int value) { outage_id = value; }
    void Trouble_Call::setType(CallType value) { type = value; }
    void Trouble_Call::setReceivedAt(TimePoint value) { received_at = value; }
    void Trouble_Call::setStatus(CallStatus value) { status = value; }

    