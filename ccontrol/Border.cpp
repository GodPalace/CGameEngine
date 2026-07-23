#include "Border.h"

void ccontrol::Border::DrawSelf(HANDLE handle)
{
    DrawRect(handle, x, y, width, height, fgColor);
}
