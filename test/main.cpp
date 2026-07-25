#include "../ccontrol/Border.h"
#include "../ccontrol/FlowPanel.h"
#include "../cgame/Console.h"
#include "../ccontrol/Text.h"
#include "../cevents/EventBus.h"

static ccontrol::FlowPanel flowPanel;
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

    flowPanel.x = 0;
    flowPanel.y = 0;
    flowPanel.width = 50;
    flowPanel.height = 50;

    text.fgColor = 0xFF0000;
    text.bgColor = 0x0000FF;
    text.width = 7;
    text.height = 1;

    border.fgColor = 0x00FF00;
    border.bgColor = 0xFF0000;
    border.width = 10;
    border.height = 10;

    flowPanel.AddChild(&text);
    flowPanel.AddChild(new ccontrol::Text("a"));
    flowPanel.AddChild(new ccontrol::Text("b"));
    flowPanel.AddChild(new ccontrol::Text("c"));
    flowPanel.AddChild(new ccontrol::Text("d"));
    flowPanel.AddChild(&border);
    cgame::SetConsoleRootControl(&flowPanel);

    cevent::RegisterListener(new A());

    Sleep(100000);
    cgame::DestroyConsole();
}
