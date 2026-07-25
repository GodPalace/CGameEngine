#include "../ccontrol/Border.h"
#include "../cgame/Console.h"
#include "../ccontrol/Text.h"
#include "../cevents/EventBus.h"

ccontrol::Text text("FPS: 0");
ccontrol::Border border;

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

int main()
{
    cgame::InitConsole();

    cgame::SetConsoleRootControl(&text);
    text.fgColor = 0xFF0000;
    text.bgColor = 0x0000FF;
    text.x = 0;
    text.y = 0;
    text.width = 7;
    text.height = 1;

    border.fgColor = 0x00FF00;
    border.bgColor = 0xFF0000;
    border.x = 10;
    border.y = 10;
    border.width = 10;
    border.height = 10;
    text.AddChild(&border);

    cevent::RegisterListener(new A());

    Sleep(100000);
    cgame::DestroyConsole();
}
