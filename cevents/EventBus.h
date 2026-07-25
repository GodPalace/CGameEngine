#pragma once

#include "IEvent.h"
#include "cstdint"

namespace cevent
{
    typedef uint32_t EventResult;
#define CEVENT_CONTINUE         0
#define CEVENT_CANCEL           1
#define CEVENT_STOP_SPEAD       2

    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual EventType GetEventType() = 0;
        virtual EventResult HandleEvent(IEvent* event) = CEVENT_CONTINUE;
    };

    EventResult DispatchEvent(IEvent* event);
    void RegisterListener(Listener* listener);
    void UnregisterListener(Listener* listener);

    void ProcessEvent();
}
