#pragma once
#include "color.hpp"
#include <string>
#include <map>
#include <tuple>
#include <list>
#include <X11/Xlib.h>

class PaintDevice;

class Painter
{
public:
    Painter(PaintDevice *);
    ~Painter();
    void drawLine(int x1, int y1, int x2, int y2);
    void drawPoint(int x, int y);
    void drawRect(int x1, int y1, int x2, int y2);
    void setColor(Color);
    void renderGlyph(wchar_t ch, int x, int y, Color fg, Color bg);
    int glyphWidth() const;
    int glyphHeight() const;
private:
    int width_;
    int height_;
    Window window_;
    Display *display_;
    GC gc_;
    int screen_;
};
