#pragma once
#include <chrono>
#include <optional>
#include <vector>


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
        Outage(int id, ElementType type, int element_id, TimePoint started_at);
        
        void addCall(int callId);
        void takeCallsFrom(const Outage& outage);
        void mergeInto(int parentId);
    
        void confirm();
        void restore(TimePoint at);
        void cancel();

        bool isOpen() const;
        bool isConfirmed() const;
        
        int getId() const;
        ElementType getType() const;
        int getElementId() const;
        OutageStatus getStatus() const;
        TimePoint getStartedAt() const;
        std::optional<TimePoint> getEndedAt() const;
        std::optional<int> getMergedId() const;
        std::vector<int> getCallIds() const;
        std::optional<int> getMergedIntoId() const;

        void setId(int i);
        void setType(ElementType t);
        void setElementId(int e);
        void setStatus(OutageStatus s);
        void setStartedAt(TimePoint sa);
        void setEndedAt(std::optional<TimePoint> sa);
        void setMergedInto(std::optional<int> mi);

    private:
        int id;
        ElementType type;
        int element_id;
        OutageStatus status;
        TimePoint started_at;
        std::optional<TimePoint> ended_at;
        std::optional<int> merged_into_id;
        std::vector<int> call_ids;
};