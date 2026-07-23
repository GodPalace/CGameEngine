#include "Screen.h"

void cgame::Screen::SetRootControl(ccontrol::Control* control)
{
    root = control;
}

ccontrol::Control* cgame::Screen::GetRootControl() const
{
    return root;
}

void cgame::Screen::Draw(HANDLE handle) const
{
    if (root == nullptr) return;
    root->Draw(handle);
}
