#include "GridPanel.h"

void ccontrol::GridPanel::UpdateChild(Control* child)
{
    child->width = (width - margin * 2 + xSpacing) / cols - xSpacing;
    child->height = (height - margin * 2 + ySpacing) / rows - ySpacing;
    child->x = x + margin + curIndex % cols * (child->width + xSpacing);
    child->y = y + margin + curIndex / cols * (child->height + ySpacing);
    curIndex++;
}
