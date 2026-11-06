#include "outage.h"

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

int Outage::getId() const { return id; }
ElementType Outage::getType() const { return type; }
int Outage::getElementId() const { return element_id; }
OutageStatus Outage::getStatus() const { return status; }
Outage::TimePoint Outage::getStartedAt() const { return started_at; }

void Outage::setType(ElementType t) { type = t; }
void Outage::setElementId(int e) { element_id = e; }
void Outage::setStatus(OutageStatus s) { status = s; }
void Outage::setStartedAt(TimePoint sa) { started_at = sa; }