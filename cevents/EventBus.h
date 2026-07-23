#pragma once

#include "IEvent.h"
#include "bemapiset.h"
#include "cstdint"

typedef uint32_t EventResult;
#define CEVENT_CONTINUE       0
#define CEVENT_CANCEL         1
#define CEVENT_STOP_SPEAD     2

typedef EventResult (CALLBACK *LISTENER)(cevent::IEvent* event);

namespace cevent
{
    EventResult DispatchEvent(IEvent* event);
    void RegisterListener(LISTENER listener);
    void UnregisterListener(LISTENER listener);

    void ProcessEvent();
}
