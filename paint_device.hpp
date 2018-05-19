#pragma once
#include <SDL.h>

class PaintDevice
{
public:
  virtual ~PaintDevice() = default;
  virtual SDL_Renderer *renderer() = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
};
