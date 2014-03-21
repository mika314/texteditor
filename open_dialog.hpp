#pragma once
#include "base_text_buffer.hpp"

class Screen;

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog(Screen *screen);
private:
    virtual void internalInsert(Coord &cursor, std::wstring);
    void scanDirectory();
    Screen *screen_;
};
