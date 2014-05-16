#pragma once
#include "base_text_buffer.hpp"

class TextScreen;

class IsearchBuffer: public BaseTextBuffer
{
public:
    IsearchBuffer(TextScreen *);
    void findNext();
private:
    TextScreen *screen_;
    std::wstring searchString_;
    Coord initialCursor_;
    bool search();
    virtual void postInsert(Coord &cursor, std::wstring);
    virtual int preBackspace(Coord &cursor, int = 1);
    virtual void postBackspace(Coord &cursor, int = 1);
};
