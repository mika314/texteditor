#include "screen.hpp"
#include "painter.hpp"
#include "resize_event.hpp"
#include "SDL2/SDL_ttf.h"

Screen::Char::Char(wchar_t ach, Color afg, Color abg):
    ch(ach),
    fg(afg),
    bg(abg)
{}

Screen::Screen(Widget *parent):
    Widget(parent),
    cursor_{0, 0}
{
    Painter p(this);
    glyphWidth_ = p.glyphWidth();
    glyphHeight_ = p.glyphHeight();
}

void Screen::paintEvent(PaintEvent &)
{
    Painter p(this);
    for (size_t y = 0; y < ch_.size(); ++y)
        for (size_t x = 0; x < ch_[y].size(); ++x)
        {
            const auto &c = ch_[y][x];
            p.renderGlyph(c.ch, x * glyphWidth_, y * glyphHeight_, c.fg, c.bg);
        }
    p.setColor(Gray);
    p.drawLine(cursor_.x * glyphWidth_, cursor_.y * glyphHeight_,
               cursor_.x * glyphWidth_, (cursor_.y + 1) * glyphHeight_);
}

void Screen::resizeEvent(ResizeEvent &e)
{
    ch_.resize(heightCh());
    for (auto &r: ch_)
        r.resize(widthCh());
}

int Screen::widthCh() const
{
    return width() / glyphWidth_;
}

int Screen::heightCh() const
{
    return height() / glyphHeight_;
}

Screen::Char &Screen::ch(int x, int y)
{
    return ch_[y][x];
}

const Screen::Char &Screen::ch(int x, int y) const
{
    return ch_[y][x];
}

Coord Screen::cursor() const
{
    return cursor_;
}

void Screen::setCursor(Coord value)
{
    cursor_ = value;
}

void Screen::setCursor(int x, int y)
{
    cursor_ = { x, y };
}
