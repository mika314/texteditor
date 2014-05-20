#include "painter.hpp"
#include "paint_device.hpp"
#include "deja_vu_sans_mono.hpp"
#include "color.hpp"
#include "application.hpp"
#include "to_utf8.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>

Painter::Painter(PaintDevice *paintDevice):
    width_(paintDevice->width()),
    height_(paintDevice->height()),
    window_(paintDevice->window()),
    display_(Application::instance()->display()),
    screen_(Application::instance()->screen())
{
    std::cout << "Painter::Painter" << std::endl;
    gc_ = XCreateGC(display_, window_, 0, 0);
    //    XClearWindow(display_, window_);
}

Painter::~Painter()
{
    std::cout << "Painter::~Painter" << std::endl;
    XFreeGC(display_, gc_);
}

void Painter::drawLine(int x1, int y1, int x2, int y2)
{
    std::cout << "draw line " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
    XDrawLine(display_, window_, gc_, x1, y1, x2, y2);
}

void Painter::drawPoint(int x, int y)
{
    std::cout << "draw point " << x << " " << y << std::endl;
    XDrawPoint(display_, window_, gc_, x, y);
}

void Painter::drawRect(int x, int y, int width, int height)
{
    std::cout << "draw rectangle " << x << " " << y << " " << width << " " << height << std::endl;
    XDrawRectangle(display_, window_, gc_, x, y, width, height);
}

void Painter::setColor(Color color)
{
    color >>= 8;
    std::cout << "set color " << std::setbase(16) << color << std::setbase(10) << std::endl;
    XSetForeground(display_, gc_, color);
    XSetBackground(display_, gc_, color);
}

void Painter::renderGlyph(wchar_t ch, int x, int y, Color fg, Color bg)
{
    std::cout << "render glyph " << x << " " << y << std::endl;
    wchar_t str[2];
    str[0] = ch;
    str[1] = 0;
    std::string tmp = toUtf8(str);
    XSetForeground(display_, gc_, fg >> 8);
    XSetBackground(display_, gc_, bg >> 8);
    XDrawString(display_, window_, gc_, x, y, tmp.c_str(), tmp.size());
}

int Painter::glyphWidth() const
{
    return 10;
}

int Painter::glyphHeight() const
{
    return 10;
}

