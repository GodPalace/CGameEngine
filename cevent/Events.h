#pragma once

#include "IEvent.h"

namespace cevent
{
    class GameTickEvent : public IEvent
    {
    public:
        bool atTickFront;

        EventType GetEventType() override
        {
            return CEVENT_GAME_TICK;
        }
    };

    class WindowEvent : public IEvent
    {
    public:
        ~WindowEvent() override = default;

        EventType GetEventType() override
        {
            return CEVENT_WINDOW;
        }
    };

    class WindowResizeEvent : public IEvent
    {
    public:
        short width;
        short height;

        EventType GetEventType() override
        {
            return CEVENT_WINDOW_RESIZE;
        }
    };

    class RenderEvent : public IEvent
    {
    public:
        EventType GetEventType() override
        {
            return CEVENT_RENDER;
        }
    };

    class KeyboardEvent : public IEvent
    {
    public:
        unsigned short vkey;
        unsigned long repeatCount;
        unsigned long scanCode;
        bool isPressed;
        bool isControlKeyDown;
        bool isAltKeyDown;
        bool isShiftKeyDown;

        EventType GetEventType() override
        {
            return CEVENT_KEYBOARD;
        }
    };

    class MouseEvent : public IEvent
    {
    public:
        ~MouseEvent() override = default;

        EventType GetEventType() override
        {
            return CEVENT_MOUSE;
        }
    };

    enum MouseButton
    {
        LEFT,
        MIDDLE,
        RIGHT
    };

    class MouseButtonEvent : public MouseEvent
    {
    public:
        MouseButton button;
        bool isRepeat;

        EventType GetEventType() override
        {
            return CEVENT_MOUSE_BUTTON;
        }
    };

    class MouseWheelEvent : public MouseEvent
    {
    public:
        short wheel;

        EventType GetEventType() override
        {
            return CEVENT_MOUSE_WHEEL;
        }
    };

    class MouseMotionEvent : public MouseEvent
    {
    public:
        int x;
        int y;

        EventType GetEventType() override
        {
            return CEVENT_MOUSE_MOTION;
        }
    };
}
