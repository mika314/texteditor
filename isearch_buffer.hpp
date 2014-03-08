#pragma once
#include "base_text_buffer.hpp"

class Screen;

class IsearchBuffer: public BaseTextBuffer
{
public:
    IsearchBuffer(Screen *);
    virtual void insert(Coord &cursor, std::wstring);
    virtual void backspace(Coord &cursor, int = 1);
    void findNext();
private:
    Screen *screen_;
    std::wstring searchString_;
    Coord initialCursor_;
    bool search();
};
