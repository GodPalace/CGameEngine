#pragma once

#include "windows.h"
#include "cstdint"
#include "../ccontrol/Control.h"

namespace cgame
{
    class Screen;

    void InitConsole(uint32_t fps = 60);
    void DestroyConsole();

    short GetConsoleWidth();
    short GetConsoleHeight();
    WORD GetConsoleAttribute();

    void SetConsoleRootControl(ccontrol::Control* control);
    ccontrol::Control* GetConsoleRootControl();

    void SetConsoleBgColor(uint32_t color);
    void SetConsoleFgColor(uint32_t color);

    void Draw();
    void Render();
    void ClearBuffer();
    void RenderAndClearBuffer();

    uint8_t GetFps();
}
