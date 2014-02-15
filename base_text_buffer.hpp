#pragma once
#include <string>
#include <vector>

class Screen;

class BaseTextBuffer
{
public:
    const std::wstring &operator[](int line) const;
    std::wstring &operator[](int line);
    int size() const;
    virtual void render(Screen *) const;
    ~BaseTextBuffer();
protected:
    std::vector<std::wstring> buffer_;
};
