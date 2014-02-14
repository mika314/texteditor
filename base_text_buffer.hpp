#pragma once
#include <string>

class Screen;

class BaseTextBuffer
{
public:
    const std::wstring &operator[](int line) const;
    std::wstring &operator[](int line);
    int size() const;
    void render(Screen *) const;
private:
    mutable std::wstring tmpLine_;
};
