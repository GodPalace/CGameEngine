#pragma once

#include "Control.h"

namespace ccontrol
{
    class Panel : public Control
    {
    public:
        void DrawSelf(HANDLE handle) override;
    };
}
