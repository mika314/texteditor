#pragma once
#include <SDL.h>

class PaintDevice
{
public:
  virtual ~PaintDevice();
  virtual SDL_Renderer *renderer() = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
};
