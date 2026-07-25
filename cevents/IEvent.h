#pragma once

#include "cstdint"

namespace cevent
{
    typedef uint8_t EventType;
#define CEVENT_GAME_TICK     0
#define CEVENT_WINDOW        1
#define CEVENT_WINDOW_RESIZE 2
#define CEVENT_RENDER        3
#define CEVENT_KEYBOARD      4
#define CEVENT_MOUSE         5
#define CEVENT_MOUSE_BUTTON  6
#define CEVENT_MOUSE_WHEEL   7
#define CEVENT_MOUSE_MOTION  8

    class IEvent
    {
    public:
        virtual ~IEvent() = default;
        virtual EventType GetEventType() = 0;
    };
}
