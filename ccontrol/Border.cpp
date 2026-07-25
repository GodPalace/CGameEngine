#include "Border.h"

void ccontrol::Border::DrawSelf(Graphics* g)
{
    g->DrawRect(x, y, width, height, fgColor, bgColor);
}
