#pragma once

#include "Control.h"

namespace ccontrol
{
    class Border : public Control
    {
    public:
        void DrawSelf(HANDLE handle) override;
    };
}
