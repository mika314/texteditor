#pragma once
#include "base_text_buffer.hpp"

class Screen;

class IsearchBuffer: public BaseTextBuffer
{
public:
    IsearchBuffer(Screen *, bool fsearch);
    void findNext();
    void findPrev();
private:
    Screen *screen_;
    std::wstring searchString_;
    Coord initialCursor_;
    bool fsearch_;
    bool search();
    bool fsearch();
    bool rsearch();
    virtual void postInsert(Coord &cursor, std::wstring);
    virtual int preBackspace(Coord &cursor, int = 1);
    virtual void postBackspace(Coord &cursor, int = 1);
};
