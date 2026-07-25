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

        short x{};
        short y{};
        short width{};
        short height{};

        void AddChild(Control* child);
        void RemoveChild(Control* child);
        std::vector<Control*> GetChildren();

        virtual ~Control() = default;
        virtual void DrawSelf(Graphics* g) = 0;

        void Draw(Graphics* g);
    };
}
