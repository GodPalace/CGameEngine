#pragma once
#include "Control.h"

namespace ccontrol
{
    class FlowPanel : public Control
    {
        void UpdateChildPos(Control* child) override;
        void DrawSelf(Graphics* g) override {}

    public:
        short xSpacing = 1;
        short ySpacing = 1;

    private:
        short nextX = xSpacing;
        short nextY = ySpacing;
        short maxHeight = 0;
    };
}
