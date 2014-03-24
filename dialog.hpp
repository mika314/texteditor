#pragma once
#include "base_text_buffer.hpp"
#include "signal_slot.hpp"
#include <string>

class Dialog: public BaseTextBuffer
{
public:
    enum Answer { Yes = 1, No = 2, Cancel = 4};
    typedef unsigned Answers;
    Dialog(std::wstring message, Answers = Yes | No | Cancel);
    Signal<void, Answer> result;
private:
    void internalInsert(Coord &cursor, std::wstring value);
    std::wstring internalBackspace(Coord &cursor, int value);
    std::wstring message_;
    Answers answers_;
};
