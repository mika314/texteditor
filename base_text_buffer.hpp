#pragma once
#include "coord.hpp"
#include <string>
#include <vector>

class Screen;

class BaseTextBuffer
{
public:
    BaseTextBuffer();
    virtual ~BaseTextBuffer();
    const std::wstring &operator[](int line) const;
    std::wstring &operator[](int line);
    int size() const;
    virtual void render(Screen *) const;
    void insert(std::wstring);
    Coord cursor() const;
    void setCursor(Coord);
    void setCursor(int x, int y);
    void del(int = 1);
    void backspace(int = 1);
protected:
    std::vector<std::wstring> buffer_;
    Coord cursor_;
};
