#include "screen.hpp"
#include "painter.hpp"
#include "SDL2/SDL_ttf.h"

Screen::Screen(Widget *parent):
    Widget(parent)
{
}

void Screen::paintEvent(PaintEvent &)
{
    Painter p(this);
    p.setColor(White);
    p.drawLine(0, 0, width(), 0);
    p.drawLine(width(), 0, width(), height());
    p.drawLine(width(), height(), 0, height());
    p.drawLine(0, height(), 0, 0);
    for (int i = L'A'; i <= L'z'; ++i)
        p.renderGlyph(i, (i - L'A') * p.glyphWidth(), 0, Black, White);
}
