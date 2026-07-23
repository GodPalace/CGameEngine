#include "Control.h"
#include "algorithm"
#include "../cgame/Console.h"

void ccontrol::Control::AddChild(Control* child)
{
    children.push_back(child);
}

void ccontrol::Control::RemoveChild(Control* child)
{
    children.erase(std::ranges::remove(children, child).begin());
}

std::vector<ccontrol::Control*> ccontrol::Control::GetChildren()
{
    return std::vector(children);
}

void ccontrol::Control::Draw(HANDLE handle)
{
    for (auto child : children)
    {
        child->Draw(handle);
    }

    DrawSelf(handle);
}

void ccontrol::Control::DrawRect(HANDLE handle, short x, short y, short w, short h, WORD attr)
{
    DWORD written;
    short realWidth = static_cast<SHORT>(std::max(0, w - std::max(0, x + w - cgame::GetConsoleWidth())));
    short realHeight = static_cast<SHORT>(std::max(0, h - std::max(0, y + h - cgame::GetConsoleHeight())));

    COORD start = {static_cast<SHORT>(x + 1), y};
    FillConsoleOutputCharacterW(handle, L'▀', realWidth - 2, start, &written);
    FillConsoleOutputAttribute(handle, attr, realWidth, start, &written);

    start = {static_cast<SHORT>(x + 1), static_cast<SHORT>(y + realHeight - 1)};
    FillConsoleOutputCharacterW(handle, L'▄', realWidth - 2, start, &written);
    FillConsoleOutputAttribute(handle, attr, realWidth, start, &written);

    for (short i = y; i < y + realHeight; i++)
    {
        start = {x, i};
        FillConsoleOutputCharacterW(handle, L'█', 1, start, &written);
        FillConsoleOutputAttribute(handle, attr, 1, start, &written);

        start = {static_cast<SHORT>(x + w - 1), i};
        FillConsoleOutputCharacterW(handle, L'█', 1, start, &written);
        FillConsoleOutputAttribute(handle, attr, 1, start, &written);
    }
}

void ccontrol::Control::FillRect(HANDLE handle, short x, short y, short w, short h, WORD attr)
{
    DWORD written;
    short realWidth = static_cast<SHORT>(std::max(0, w - std::max(0, x + w - cgame::GetConsoleWidth())));
    short realHeight = static_cast<SHORT>(std::max(0, h - std::max(0, y + h - cgame::GetConsoleHeight())));

    for (short i = y; i < y + realHeight; i++)
    {
        COORD start = {x, i};
        FillConsoleOutputCharacterW(handle, L'█', realWidth, start, &written);
        FillConsoleOutputAttribute(handle, attr, realWidth, start, &written);
    }
}
