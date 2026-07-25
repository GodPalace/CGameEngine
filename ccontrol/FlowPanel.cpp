#include "FlowPanel.h"

void ccontrol::FlowPanel::UpdateChildPos(Control* child)
{
    if (nextX + child->width + xSpacing <= width)
    {
        maxHeight = std::max(maxHeight, child->height);
    }
    else
    {
        nextX = xSpacing;
        nextY += maxHeight + ySpacing;
        maxHeight = child->height;
    }

    child->x = nextX;
    child->y = nextY;
    nextX += child->width + xSpacing;
}
