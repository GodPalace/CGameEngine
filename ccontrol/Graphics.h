#pragma once

#include "windows.h"
#include "string"
#include "cstdint"

namespace ccontrol
{
    class Graphics
    {
        HANDLE handle;

    public:
        explicit Graphics(HANDLE buffer);

        void DrawRect(short x, short y, short w, short h, uint32_t fgColor, uint32_t bgColor) const;
        void FillRect(short x, short y, short w, short h, uint32_t fgColor) const;

#undef DrawText
        void DrawText(const std::string& text, short x, short y, uint32_t fgColor, uint32_t bgColor) const;
    };
}
