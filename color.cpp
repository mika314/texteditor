#include "color.hpp"

SDL_Color toSdlColor(Color value)
{
  return { static_cast<Uint8>((value >> 24) & 0xff), 
      static_cast<Uint8>((value >> 16) & 0xff), 
      static_cast<Uint8>((value >> 8) & 0xff), 
      static_cast<Uint8>(value & 0xff)};
}
