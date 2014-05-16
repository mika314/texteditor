#include "painter.hpp"
#include "paint_device.hpp"
#include "deja_vu_sans_mono.hpp"
#include "color.hpp"
#include <stdexcept>

Painter::Painter(PaintDevice *paintDevice):
    width_(paintDevice->width()),
    height_(paintDevice->height())
{
}

Painter::~Painter()
{
}

void Painter::drawLine(int x1, int y1, int x2, int y2)
{
}

void Painter::drawPoint(int x, int y)
{
}

void Painter::drawRect(int x, int y, int width, int height)
{
}

void Painter::setColor(Color color)
{
}

void Painter::renderGlyph(wchar_t ch, int x, int y, Color fg, Color bg)
{
}

int Painter::glyphWidth() const
{
    return 10;
}

int Painter::glyphHeight() const
{
    return 10;
}

