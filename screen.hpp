#pragma once
#include "widget.hpp"

class Screen: public Widget
{
public:
    Screen(Widget * = nullptr);
private:
    virtual void paintEvent(PaintEvent &);
};
