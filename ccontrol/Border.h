#pragma once

#include "Control.h"

namespace ccontrol
{
    class Border : public Control
    {
        void DrawSelf(Graphics* g) override;
        void UpdateChildPos(Control* child) override {}
    };
}
