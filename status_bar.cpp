#include "status_bar.hpp"

StatusBar::StatusBar(Widget *parent):
    Screen(parent)
{}

int StatusBar::maxHeight() const
{
    return 20; // TODO
}

int StatusBar::minHeight() const
{
    return 20; // TODO
}

