#pragma once

#include "windows.h"
#include "vector"
#include "../cevents/EventBus.h"

namespace ccontrol
{
    class Control
    {
        std::vector<Control*> children;

    public:
        WORD fgColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

        short x{};
        short y{};
        short width{};
        short height{};

        void AddChild(Control* child);
        void RemoveChild(Control* child);
        std::vector<Control*> GetChildren();

        virtual ~Control() = default;
        virtual void DrawSelf(HANDLE handle) = 0;

        void Draw(HANDLE handle);
        static void DrawRect(HANDLE handle, short x, short y, short w, short h, WORD attr);
        static void FillRect(HANDLE handle, short x, short y, short w, short h, WORD attr);
    };
}
