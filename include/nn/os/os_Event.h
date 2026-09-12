#pragma once

#include <nn/os/os_EventCommon.h>
#include <nn/os/os_EventTypes.h>
#include <nn/time.h>
#include <nn/util.h>

namespace nn::os {

// todo: figure out where these go
void InitializeEvent(EventType*, bool initiallySignaled, EventClearMode eventClearMode);
void FinalizeEvent(EventType*);
void SignalEvent(EventType*);
void WaitEvent(EventType*);
bool TryWaitEvent(EventType*);
bool TimedWaitEvent(EventType*, nn::TimeSpan);
void ClearEvent(EventType*);

class Event {
    NN_NO_COPY(Event);
    NN_NO_MOVE(Event);

public:
    explicit Event(EventClearMode clearMode) { InitializeEvent(&m_Event, false, clearMode); }

    ~Event() { FinalizeEvent(&m_Event); }

    void Wait() { WaitEvent(&m_Event); }

    bool TryWait() { return TryWaitEvent(&m_Event); }

    bool TimedWait(TimeSpan timeSpan) { return TimedWaitEvent(&m_Event, timeSpan); }

    void Signal() { SignalEvent(&m_Event); }

    void Clear() { ClearEvent(&m_Event); }

    operator EventType&() { return m_Event; }

    operator const EventType&() const { return m_Event; }

    EventType* GetBase() { return &m_Event; }

private:
    EventType m_Event;
};

}  // namespace nn::os