#pragma once
#include "color.hpp"
#include <vector>

class BaseTextBuffer;

class CppHighlighter
{
public:
    CppHighlighter(BaseTextBuffer *);
    Color fgColor(int x, int y) const;
    Color bgColor(int x, int y) const;
    void update(int x, int y);
private:
    enum Type { Keyword, Indent, Comment, Macro, StringLiteral, Number, Other };
    std::vector<std::vector<std::pair<Color, Color> > > colors_;
    BaseTextBuffer *textBuffer_;
    std::pair<Type, int> getToken(int &x, int &y);
};
