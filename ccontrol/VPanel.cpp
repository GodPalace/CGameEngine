#include "VPanel.h"

void ccontrol::VPanel::UpdateChild(Control* child)
{
    child->width = width - margin * 2;
    child->x = x + margin;

    if (direction == TOP_TO_BOTTOM)
    {
        child->y = y + margin + curY;
        curY += child->height + spacing;
    }
    else
    {
        child->y = y + height - margin - curY - child->height;
        curY += child->height + spacing;
    }
}
