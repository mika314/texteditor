#pragma once
#include "base_text_buffer.hpp"

class Screen;

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog(Screen *screen);
    virtual void insert(Coord &cursor, std::wstring);
private:
    Screen *screen_;
};
