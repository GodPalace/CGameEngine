#pragma once

#include "Graphics.h"
#include "cstdint"
#include "vector"

namespace ccontrol
{
    class Control
    {
        std::vector<Control*> children;

    public:
        uint32_t fgColor = 0xFFFFFF;
        uint32_t bgColor = 0x000000;

        short x = 0;
        short y = 0;
        short width = 10;
        short height = 10;

        void AddChild(Control* child);
        void RemoveChild(Control* child);
        std::vector<Control*> GetChildren();

        virtual ~Control() = default;

        void Draw(Graphics* g);

    private:
        virtual void UpdateChildPos(Control* child) = 0;

        virtual void DrawSelf(Graphics* g) = 0;
        void DrawBg(const Graphics* g) const;
    };
}
