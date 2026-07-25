#include "Graphics.h"
#include "../cgame/Console.h"

ccontrol::Graphics::Graphics(HANDLE buffer)
{
    handle = buffer;
}

// 返回一个 ANSI 真彩色前缀串，bg=true 时改背景
static std::string Rgb(uint32_t color, bool bg = false) {
    uint8_t r = color >> 16 & 0xFF;
    uint8_t g = color >> 8 & 0xFF;
    uint8_t b = color & 0xFF;
    return "\033[" + std::to_string(bg ? 48 : 38) + ";2;"
         + std::to_string(r) + ";"
         + std::to_string(g) + ";"
         + std::to_string(b) + "m";
}

static std::string Reset()
{
    return "\033[0m";
}

static void SetColorAt(HANDLE handle, const std::string& color, short x, short y)
{
    DWORD written;
    COORD start = {.X = x, .Y = y};
    SetConsoleCursorPosition(handle, start);
    WriteConsoleA(handle, color.c_str(), color.length(), &written, nullptr);
}

static void ResetAt(HANDLE handle, short x, short y)
{
    DWORD written;
    COORD start = {.X = x, .Y = y};

    SetConsoleCursorPosition(handle, start);
    WriteConsoleA(handle, Reset().c_str(), Reset().length(), &written, nullptr);
}

static void FillPrintCharAt(HANDLE handle, wchar_t c, short x, short y, uint8_t count)
{
    DWORD written;
    auto str = std::wstring(count, c);
    SetConsoleCursorPosition(handle, {.X = x, .Y = y});
    WriteConsoleW(handle, str.c_str(), str.length(), &written, nullptr);
}

void ccontrol::Graphics::DrawRect(short x, short y, short w, short h, uint32_t fgColor, uint32_t bgColor) const
{
    SetColorAt(handle, Rgb(fgColor) + Rgb(bgColor, true), x, y);
    FillPrintCharAt(handle, L'▀', x + 1, y, w - 2);
    for (short i = y; i < y + h; i++)
    {
        FillPrintCharAt(handle, L'█', x, i, 1);
        FillPrintCharAt(handle, L'█', x + w - 1, i, 1);
    }
    FillPrintCharAt(handle, L'▄', x + 1, y + h - 1, w - 2);

    ResetAt(handle, x + w, y + h);
}

void ccontrol::Graphics::FillRect(short x, short y, short w, short h, uint32_t fgColor) const
{
    SetColorAt(handle, Rgb(fgColor), x, y);
    for (short i = y; i < y + h; i++)
    {
        FillPrintCharAt(handle, L'█', x, i, w);
    }
    ResetAt(handle, x + w, y + h);
}

void ccontrol::Graphics::DrawText(const std::string& text, short x, short y, uint32_t fgColor, uint32_t bgColor) const
{
    DWORD written;
    std::string out = Rgb(fgColor) + Rgb(bgColor, true) + text + Reset();
    SetConsoleCursorPosition(handle, {.X = x, .Y = y});
    WriteConsoleA(handle, out.c_str(), out.length(), &written, nullptr);
}
