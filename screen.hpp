#pragma once
#include "widget.hpp"
#include "color.hpp"
#include "coord.hpp"

class Screen: public Widget
{
public:
    Screen(Widget * = nullptr);
    struct Char
    {
        Char(wchar_t = L'\0', Color fg = Black, Color bg = White);
        wchar_t ch;
        Color fg;
        Color bg;
    };
    int widthCh() const;
    int heightCh() const;
    Char &ch(int x, int y);
    const Char &ch(int x, int y) const;
    Coord cursor() const;
    void setCursor(Coord);
    void setCursor(int x, int y);
private:
    int glyphWidth_;
    int glyphHeight_;
    Coord cursor_;
    virtual void paintEvent(PaintEvent &);
    std::vector<std::vector<Char> > ch_;
protected:
    virtual void resizeEvent(ResizeEvent &e);
};
