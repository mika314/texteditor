#pragma once
#include "base_text_buffer.hpp"
#include "signal_slot.hpp"

class Screen;

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog(Screen *screen);
  Signal<void (OpenDialog *, std::string)> openFile;
private:
    Screen *screen_;
    void scanDirectory();
    virtual void postInsert(Coord &cursor, std::wstring);
};
