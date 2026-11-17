#include "outage.h"
#include <stdexcept>
#include <algorithm>

Outage::Outage()
    : id(0),
      type(ElementType::ServicePoint),
      element_id(0),
      status(OutageStatus::Predicted),
      started_at(std::chrono::system_clock::now()){}

Outage::Outage(int id, ElementType type, int element_id,
               OutageStatus status, TimePoint started_at)
    : id(id),
      type(type),
      element_id(element_id),
      status(status),
      started_at(started_at){}

void Outage::addCall(int callId){
  if (!isOpen()) {
    throw std::logic_error("Cannot add a call to an outage that is not open");
  }
  if (std::find(call_ids.begin(), call_ids.end(), callId) != call_ids.end()) {
    return;
  }
  call_ids.push_back(callId);
}

void Outage::takeCallsFrom(const Outage& outage){
  for (int callId : outage.getCallIds()) {
    addCall(callId);
  }
}
    
void Outage::confirm(){
  if(status != OutageStatus::Predicted){
    throw std::logic_error("Only a predicted outage can be confirmed");
  }
  status = OutageStatus::Confirmed;
}

void Outage::restore(TimePoint at){
  if(!isOpen()){
    throw std::logic_error("Outage is already restored or cancelled");
  }
  status = OutageStatus::Restored;
  ended_at = at;
}

void Outage::cancel(){
  if(!isOpen()){
    throw std::logic_error("Outage is already restored or cancelled");
  }
  status = OutageStatus::Cancelled;
}

bool Outage::isOpen() const{
  return status == OutageStatus::Confirmed || status == OutageStatus::Predicted;
}

bool Outage::isConfirmed() const{
  return status == OutageStatus::Confirmed;
}

int Outage::getId() const { return id; }
ElementType Outage::getType() const { return type; }
int Outage::getElementId() const { return element_id; }
OutageStatus Outage::getStatus() const { return status; }
Outage::TimePoint Outage::getStartedAt() const { return started_at; }
std::optional<Outage::TimePoint> Outage::getEndedAt() const { return ended_at; }
std::optional<int> Outage::getMergedId() const { return merged_into_id; }
std::vector<int> Outage::getCallIds() const { return call_ids; }

void Outage::setType(ElementType t) { type = t; }
void Outage::setElementId(int e) { element_id = e; }
void Outage::setStatus(OutageStatus s) { status = s; }
void Outage::setStartedAt(TimePoint sa) { started_at = sa; }
void Outage::setEndedAt(std::optional<TimePoint> ea) { ended_at = ea; }