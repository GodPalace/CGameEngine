#pragma once

#include "Panel.h"

namespace ccontrol
{
    class GridPanel : public Panel
    {
        void UpdateChild(Control* child) override;
        void DrawSelf(Graphics* g) override {}

    public:
        short rows = 1;
        short cols = 1;
        short margin = 0;
        short xSpacing = 0;
        short ySpacing = 0;

    private:
        short curIndex = 0;
    };
}
