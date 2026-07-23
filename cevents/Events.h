#pragma once

#include "IEvent.h"

namespace cevent
{
    class GameTickEvent : public IEvent
    {
    public:
        bool atTickFront;
    };

    class WindowEvent : public IEvent
    {
    public:
        ~WindowEvent() override = default;
    };

    class WindowResizeEvent : public IEvent
    {
    public:
        short width;
        short height;
    };

    class RenderEvent : public IEvent
    {
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
    };

    class MouseEvent : public IEvent
    {
    public:
        ~MouseEvent() override = default;
    };

    class MouseButtonEvent : public MouseEvent
    {
    public:
        unsigned long button;
        bool isPressed;
        bool isRepeat;
    };

    class MouseWheelEvent : public MouseEvent
    {
    public:
        short wheel;
    };

    class MouseMotionEvent : public MouseEvent
    {
    public:
        int x;
        int y;
    };
}
