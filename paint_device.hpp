#pragma once
#include <SDL2/SDL.h>

class PaintDevice
{
public:
    virtual ~PaintDevice();
    virtual int width() const = 0;
    virtual int height() const = 0;
};
