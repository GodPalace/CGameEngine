#pragma once

#include "Control.h"
#include "string"

namespace ccontrol
{
    class Text : public Control
    {
        std::string text;
        short strLenOfChar;

    public:
        explicit Text(const std::string& str) : text(str), strLenOfChar(CalcStrWidth(str)) {}

        std::string GetText() const;
        void SetText(const std::string& str);

        void DrawSelf(Graphics* g) override;

    private:
        static short CalcStrWidth(const std::string& s);
    };
}
