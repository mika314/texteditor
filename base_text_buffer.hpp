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
    void insert(Coord &cursor, std::wstring);
    void del(const Coord cursor, int = 1);
    void backspace(Coord &cursor, int = 1);
protected:
    std::vector<std::wstring> buffer_;
};
