#include "../ccontrol/Border.h"
#include "../ccontrol/GridPanel.h"
#include "../cgame/Console.h"
#include "../ccontrol/Text.h"
#include "../cevent/EventBus.h"

static ccontrol::GridPanel panel;
static ccontrol::Text text("FPS: 0");
static ccontrol::Border border;

namespace
{
    class A : public cevent::Listener
    {
    public:
        cevent::EventType GetEventType() override
        {
            return CEVENT_RENDER;
        }

        cevent::EventResult HandleEvent(cevent::IEvent* event) override
        {
            text.SetText("FPS: " + std::to_string(cgame::GetFps()));
            return CEVENT_CONTINUE;
        }
    };
}

int main()
{
    cgame::InitConsole();

    panel.xSpacing = 1;
    panel.ySpacing = 1;
    panel.rows = 2;
    panel.cols = 3;
    panel.x = 0;
    panel.y = 0;
    panel.width = 30;
    panel.height = 15;

    text.fgColor = 0xFF0000;
    text.bgColor = 0x0000FF;
    text.width = 7;
    text.height = 1;

    border.fgColor = 0x00FF00;
    border.bgColor = 0xFF0000;
    border.width = 10;
    border.height = 10;

    panel.AddChild(&text);
    panel.AddChild(new ccontrol::Border());
    panel.AddChild(new ccontrol::Border());
    panel.AddChild(new ccontrol::Border());
    panel.AddChild(new ccontrol::Border());
    panel.AddChild(&border);
    cgame::SetConsoleRootControl(&panel);

    cevent::RegisterListener(new A());

    Sleep(100000);
    cgame::DestroyConsole();
}
