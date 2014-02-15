#pragma once
#include <string>

class Screen;

class BaseTextBuffer
{
public:
    virtual const std::wstring &operator[](int line) const = 0;
    virtual std::wstring &operator[](int line) = 0;
    virtual int size() const = 0;
    virtual void render(Screen *) const;
    ~BaseTextBuffer();
};
