#include "EventBus.h"
#include "algorithm"
#include "Events.h"
#include "vector"
#include "windows.h"
#include "stdexcept"

std::vector<cevent::Listener*> g_listeners;
HANDLE g_hIn = nullptr;
INPUT_RECORD g_ir[32];
DWORD g_events;

cevent::EventResult cevent::DispatchEvent(IEvent* event)
{
    if (g_listeners.empty()) return CEVENT_CONTINUE;

    for (auto listener : g_listeners)
    {
        if (listener->GetEventType() != event->GetEventType()) continue;
        EventResult result = listener->HandleEvent(event);
        if (result != CEVENT_CONTINUE) return result;
    }

    return CEVENT_CONTINUE;
}

void cevent::RegisterListener(Listener* listener)
{
    g_listeners.push_back(listener);
}

void cevent::UnregisterListener(Listener* listener)
{
    g_listeners.erase(std::ranges::remove(g_listeners, listener).begin(), g_listeners.end());
}

void cevent::ProcessEvent()
{
    if (g_hIn == nullptr)
    {
        g_hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (g_hIn == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error("Failed to get input handle");
        }
    }

    while (PeekConsoleInput(g_hIn, g_ir, 32, &g_events) && g_events > 0)
    {
        ReadConsoleInput(g_hIn, g_ir, 32, &g_events);

        for (int i = 0; i < g_events; i++)
        {
            if (g_ir[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
            {
                auto [X, Y] = g_ir[i].Event.WindowBufferSizeEvent.dwSize;
                WindowResizeEvent event;
                event.width = X;
                event.height = Y;
                DispatchEvent(&event);
            }
            else if (g_ir[i].EventType == KEY_EVENT)
            {
                auto &key = g_ir[i].Event.KeyEvent;
                KeyboardEvent event;
                event.vkey = key.wVirtualKeyCode;
                event.repeatCount = key.wRepeatCount;
                event.scanCode = key.wVirtualScanCode;
                event.isPressed = key.bKeyDown;
                event.isControlKeyDown = key.dwControlKeyState & LEFT_CTRL_PRESSED;
                event.isAltKeyDown = key.dwControlKeyState & LEFT_ALT_PRESSED;
                event.isShiftKeyDown = key.dwControlKeyState & SHIFT_PRESSED;
                DispatchEvent(&event);
            }
            else if (g_ir[i].EventType == MOUSE_EVENT)
            {
                auto &mouse = g_ir[i].Event.MouseEvent;

                if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    MouseButtonEvent event;
                    event.button = mouse.dwButtonState;
                    event.isPressed = true;
                    event.isRepeat = false;
                    DispatchEvent(&event);
                }
            }
        }
    }
}
