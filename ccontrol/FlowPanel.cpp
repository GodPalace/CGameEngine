#include "FlowPanel.h"

void ccontrol::FlowPanel::UpdateChild(Control* child)
{
    if (margin * 2 + curX + child->width <= width)
    {
        maxHeight = std::max(maxHeight, child->height);
    }
    else
    {
        curX = 0;
        curY += maxHeight + ySpacing;
        maxHeight = child->height;
    }

    child->x = x + margin + curX;
    child->y = y + margin + curY;
    curX += child->width + xSpacing;
}
