#pragma once

#include "Panel.h"

namespace ccontrol
{
    enum VPanelDirection
    {
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP
    };

    class VPanel : public Panel
    {
        VPanelDirection direction = TOP_TO_BOTTOM;

        void UpdateChild(Control* child) override;
        void DrawSelf(Graphics* g) override {}

    public:
        explicit VPanel(VPanelDirection direction) : direction(direction) {}

        short margin = 0;
        short spacing = 0;

    private:
        short curY = 0;
    };
}
