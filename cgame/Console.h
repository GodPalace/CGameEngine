#pragma once

#include "windows.h"
#include "cstdint"

namespace cgame
{
    class Screen;

    // 当fps=0时不限制帧率
    void InitConsole(int fps = 30);
    void DestroyConsole();

    short GetConsoleWidth();
    short GetConsoleHeight();
    WORD GetConsoleAttribute();

    void SetScreen(Screen* screen);
    Screen* GetScreen();

    void SetConsoleBgColor(uint32_t color);
    void SetConsoleFgColor(uint32_t color);

    void Draw();
    void Render();
    void ClearBuffer();
    void RenderAndClearBuffer();

    uint8_t GetFps();
}
