#include "painter.hpp"
#include "paint_device.hpp"

Painter::Painter(PaintDevice *paintDevice):
    renderer_(paintDevice->renderer()),
    gLeft_(paintDevice->gLeft()),
    gTop_(paintDevice->gTop())
{
}

void Painter::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer_, gLeft_ + x1, gTop_ + y1, gLeft_ + x2, gTop_ + y2);
}

void Painter::drawPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer_, gLeft_ + x, gTop_ + y);
}

void Painter::drawRect(int x, int y, int width, int height)
{
    SDL_Rect rect = { x + gLeft_, y + gTop_, width, height };
    SDL_RenderFillRect(renderer_, &rect);
}

void Painter::setColor(Color color)
{
    SDL_SetRenderDrawColor(renderer_, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
}
