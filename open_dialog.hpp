#pragma once
#include "base_text_buffer.hpp"

class OpenDialog: public BaseTextBuffer
{
public:
    OpenDialog();
    virtual void insert(Coord &cursor, std::wstring);
};
