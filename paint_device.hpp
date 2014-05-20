#pragma once
#include <X11/Xlib.h>

class PaintDevice
{
public:
    virtual ~PaintDevice();
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual Window window() const = 0;
};
