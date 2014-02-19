#pragma once
#include <string>

class TextInputEvent
{
public:
    TextInputEvent(std::wstring);
    std::wstring text() const;
private:
    std::wstring text_;
};
