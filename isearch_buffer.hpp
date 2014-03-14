#pragma once
#include "base_text_buffer.hpp"

class Screen;

class IsearchBuffer: public BaseTextBuffer
{
public:
    IsearchBuffer(Screen *);
    void findNext();
private:
    virtual void internalInsert(Coord &cursor, std::wstring);
    virtual std::wstring internalBackspace(Coord &cursor, int = 1);
    Screen *screen_;
    std::wstring searchString_;
    Coord initialCursor_;
    bool search();
};
