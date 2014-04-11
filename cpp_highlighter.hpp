#pragma once
#include "color.hpp"
#include <vector>
#include <set>
#include <string>

class BaseTextBuffer;

class CppHighlighter
{
public:
    CppHighlighter(BaseTextBuffer *);
    Color fgColor(int x, int y) const;
    Color bgColor(int x, int y) const;
    void update(int x, int y);
private:
    enum Type { Keyword, Ident, Comment, Macro, StringLiteral, Number, Other };
    std::vector<std::vector<std::pair<Color, Color> > > colors_;
    BaseTextBuffer *textBuffer_;
    std::set<std::string> keywords_;
    std::pair<Type, int> getToken(int &x, int &y);
    void moveForward(int &x, int &y) const;
    void moveBackward(int &x, int &y) const;
    bool outOfRange(int x, int y) const;
    wchar_t ch(int x, int y) const;
};
