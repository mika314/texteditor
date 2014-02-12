#pragma once
#include "color.hpp"
#include <SDL2/SDL.h>

class PaintDevice;

class Painter
{
public:
    Painter(PaintDevice *);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawPoint(int x, int y);
    void drawRect(int x1, int y1, int x2, int y2);
    void setColor(Color);
private:
    SDL_Renderer *renderer_;
    int gLeft_;
    int gTop_;
};
