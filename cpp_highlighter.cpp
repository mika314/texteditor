#include "cpp_highlighter.hpp"
#include "base_text_buffer.hpp"
#include <cwctype>
#include <iostream>

// keywords
// indents
// comments
// macro
// string literal

CppHighlighter::CppHighlighter(BaseTextBuffer *textBuffer):
    textBuffer_(textBuffer)
{
    update(0, 0);
}

Color CppHighlighter::fgColor(int x, int y) const
{
    if (y < static_cast<int>(colors_.size()))
        if (x < static_cast<int>(colors_[y].size()))
            return colors_[y][x].first;
    return Black;
}

Color CppHighlighter::bgColor(int x, int y) const
{
    if (y < static_cast<int>(colors_.size()))
        if (x < static_cast<int>(colors_[y].size()))
            return colors_[y][x].second;
    return White;
}

std::pair<CppHighlighter::Type, int> CppHighlighter::getToken(int &x, int &y)
{
    int count = 0;
    for (int i = 0; i < 10; ++i)
    {
        ++x;
        ++count;
        if (y > static_cast<int>(textBuffer_->size()) - 1 || 
            (y == static_cast<int>(textBuffer_->size()) - 1 && 
             x >= static_cast<int>((*textBuffer_)[textBuffer_->size() - 1].size()) - 1))
            break;
        while (x >= static_cast<int>((*textBuffer_)[y].size()))
        {
            x = 0;
            ++y;
            if (y > static_cast<int>(textBuffer_->size()) - 1 || 
                (y == static_cast<int>(textBuffer_->size()) - 1 && 
                 x >= static_cast<int>((*textBuffer_)[textBuffer_->size() - 1].size()) - 1))
                break;
        }
    }
    return std::make_pair(static_cast<Type>(x % 7), count);
}

void CppHighlighter::update(int, int)
{
    colors_.resize(textBuffer_->size());
    for (int y = 0; y < textBuffer_->size(); ++y)
    {
        auto &line = (*textBuffer_)[y];
        colors_[y].resize(line.size());
    }
    
    int x = 0; 
    int y = 0;
    for (;;)
    {
        std::pair<Type, int> token = getToken(x, y);
        std::pair<Color, Color> color;
        switch (token.first)
        {
        case Keyword:
            color = std::make_pair(Green, White);
            break;
        case Indent:
            color = std::make_pair(Black, White);
            break;
        case Comment:
            color = std::make_pair(Brown, White); 
            break;
        case Macro:
            color = std::make_pair(Violetred, White);
            break;
        case StringLiteral:
            color = std::make_pair(Steelblue, White);
            break;
        case Number:
            color = std::make_pair(Blue, White);
            break;
        case Other:
            color = std::make_pair(Black, White);
            break;
        }
        int count = token.second;
        int tmpX = x;
        int tmpY = y;
        for (; count > 0; --count)
        {
            --tmpX;
            while (tmpX < 0)
            {
                --tmpY;
                tmpX = colors_[tmpY].size() - 1;
            }
            // if (tmpY < static_cast<int>(textBuffer_->size()) - 1 ||
            //     (y == static_cast<int>(textBuffer_->size()) - 1 &&
            //      x < static_cast<int>((*textBuffer_)[textBuffer_->size() - 1].size()) - 1))
            {
                colors_[tmpY][tmpX] = color;
            }
        }
        if (y > static_cast<int>(textBuffer_->size()) - 1 || 
            (y == static_cast<int>(textBuffer_->size()) - 1 && 
             x >= static_cast<int>((*textBuffer_)[textBuffer_->size() - 1].size()) - 1))
            break;
    }
}
