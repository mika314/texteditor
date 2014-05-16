#pragma once
#include "base_text_buffer.hpp"
#include "signal_slot.hpp"

class TextScreen;

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog(TextScreen *screen);
    Signal<void, OpenDialog *, std::string> openFile;
private:
    TextScreen *screen_;
    void scanDirectory();
    virtual void postInsert(Coord &cursor, std::wstring);
};
