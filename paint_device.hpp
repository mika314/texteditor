#pragma once
#include <SDL2/SDL.h>

class PaintDevice
{
public:
    virtual ~PaintDevice();
    virtual SDL_Renderer *renderer() = 0;
    virtual int gLeft() const = 0;
    virtual int gTop() const = 0;
};
