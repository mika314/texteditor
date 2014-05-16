#include "status_bar.hpp"

StatusBar::StatusBar(Widget *parent):
    TextScreen(parent)
{}

int StatusBar::maxHeight() const
{
    return glyphHeight();
}

int StatusBar::minHeight() const
{
    return glyphHeight();
}

