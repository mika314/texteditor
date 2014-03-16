#pragma once
#include "base_text_buffer.hpp"
#include <memory>

class Screen;
class TextFile;

class SaveDialog: public BaseTextBuffer
{
public:
    SaveDialog(Screen *screen, std::shared_ptr<TextFile> textFile);
private:
    virtual void internalInsert(Coord &cursor, std::wstring);
    virtual std::wstring internalDelete(const Coord cursor, int = 1);
    virtual std::wstring internalBackspace(Coord &cursor, int = 1);
    Screen *screen_;
    std::shared_ptr<TextFile> textFile_;
    Coord cursor_;
};
