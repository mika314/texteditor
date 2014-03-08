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
    virtual void insert(Coord &cursor, std::wstring);
    virtual void del(const Coord cursor, int = 1);
    virtual void backspace(Coord &cursor, int = 1);
    bool isReadOnly() const;
    void setReadOnly(bool);
protected:
    std::vector<std::wstring> buffer_;
    bool isReadOnly_;
};
