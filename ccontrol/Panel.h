#pragma once

#include "Control.h"

namespace ccontrol
{
    class Panel : public Control
    {
        void DrawSelf(Graphics* g) override {}
    };
}
