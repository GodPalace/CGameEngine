#include "../ccontrol/Border.h"
#include "../cgame/Console.h"
#include "../cgame/Screen.h"
#include "../ccontrol/Text.h"
#include "../cevents/Events.h"
#include "typeinfo"

ccontrol::Text text = "FPS: 0";

EventResult RenderListener(cevent::IEvent* event)
{
    if (typeid(*event) != typeid(cevent::RenderEvent)) return CEVENT_CONTINUE;
    text.SetText(("FPS: " + std::to_string(cgame::GetFps())).c_str());
    return CEVENT_CONTINUE;
}

int main()
{
    cgame::InitConsole();

    cgame::Screen screen;

    screen.SetRootControl(&text);
    cgame::SetScreen(&screen);
    text.x = 10;
    text.y = 10;
    text.width = 10;
    text.height = 10;

    cevent::RegisterListener(RenderListener);

    Sleep(100000);
    cgame::DestroyConsole();
}
