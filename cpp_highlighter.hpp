#pragma once
#include "color.hpp"
#include "coord.hpp"
#include <vector>
#include <set>
#include <string>
#include <map>

class BaseTextBuffer;

class CppHighlighter
{
public:
    CppHighlighter(BaseTextBuffer *);
    Color fgColor(int x, int y) const;
    Color bgColor(int x, int y) const;
    void update(const Coord &start, const Coord &end);
private:
    enum Type { Keyword, Ident, Comment, Macro, StringLiteral, Number, Other };
    std::vector<std::vector<Type> > types_;
    BaseTextBuffer *textBuffer_;
    std::set<std::string> keywords_;
    std::pair<Type, int> getToken(int &x, int &y);
    void moveForward(int &x, int &y) const;
    void moveBackward(int &x, int &y) const;
    bool outOfRange(int x, int y) const;
    wchar_t ch(int x, int y) const;
    mutable std::map<Type, Color> toFg_;
    mutable std::map<Type, Color> toBg_;
};
