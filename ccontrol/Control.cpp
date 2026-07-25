#include "Control.h"
#include "algorithm"

void ccontrol::Control::AddChild(Control* child)
{
    children.push_back(child);
}

void ccontrol::Control::RemoveChild(Control* child)
{
    children.erase(std::ranges::remove(children, child).begin());
}

std::vector<ccontrol::Control*> ccontrol::Control::GetChildren()
{
    return std::vector(children);
}

void ccontrol::Control::Draw(Graphics* g)
{
    for (auto child : children)
    {
        child->Draw(g);
    }

    DrawSelf(g);
}
