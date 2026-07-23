#pragma once

#include "../ccontrol/Control.h"
#include "windows.h"

namespace cgame
{
    class Screen
    {
        ccontrol::Control* root{};

    public:
        Screen() = default;

        void SetRootControl(ccontrol::Control* control);
        ccontrol::Control* GetRootControl() const;

        void Draw(HANDLE handle) const;
    };
}
