#pragma once
#include <chrono>
#include <optional>

enum class ElementType {
    ServicePoint,
    Feeder,
    Transformer
};

enum class OutageStatus {
    Predicted,
    Confirmed,
    Restored,
    Merged,
    Cancelled
};

class Outage{
    public:
        using TimePoint = std::chrono::system_clock::time_point;
        
        Outage();
        Outage(int id, ElementType type, int element_id,
        OutageStatus status, TimePoint started_at);
        
        int getId() const;
        ElementType getType() const;
        int getElementId() const;
        OutageStatus getStatus() const;
        TimePoint getStartedAt() const;
        std::optional<TimePoint> getEndedAt() const;

        void setType(ElementType t);
        void setElementId(int e);
        void setStatus(OutageStatus s);
        void setStartedAt(TimePoint sa);
        void setEndedAt(std::optional<TimePoint> sa);

    private:
        int id;
        ElementType type;
        int element_id;
        OutageStatus status;
        TimePoint started_at;
        std::optional<TimePoint> ended_at;
};