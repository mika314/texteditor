#pragma once
#include "base_text_buffer.hpp"
#include "signal_slot.hpp"

class Screen;

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog(Screen *screen);
    Signal<void, OpenDialog *, std::string> openFile;
private:
    virtual void internalInsert(Coord &cursor, std::wstring);
    void scanDirectory();
    Screen *screen_;
};
