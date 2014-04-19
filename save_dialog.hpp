#pragma once
#include "base_text_buffer.hpp"
#include "signal_slot.hpp"

class Screen;
class TextFile;

class SaveDialog: public BaseTextBuffer
{
public:
    SaveDialog(Screen *screen, TextFile *textFile);
    Signal<void, SaveDialog *, TextFile *, std::string> saveAs;
private:
    Screen *screen_;
    TextFile *textFile_;
    void scanDirectory();
    virtual std::wstring preInsert(Coord &cursor, std::wstring);
    virtual int preDelete(const Coord cursor, int = 1);
    virtual int preBackspace(Coord &cursor, int = 1);
};
