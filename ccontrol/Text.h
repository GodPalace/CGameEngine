#pragma once

#include "Control.h"
#include "string"

namespace ccontrol
{
    class Text : public Control
    {
        const char* text;
        short strLenOfChar;
        short strLen;

    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Text(const char* str) : text(str), strLenOfChar(CalcStrWidth(str)), strLen(strlen(str)) {}

        const char* GetText() const;
        void SetText(const char* str);

        void DrawSelf(HANDLE handle) override;

    private:
        static short CalcStrWidth(const std::string& s);
    };
}
