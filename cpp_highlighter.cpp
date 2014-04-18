#include "cpp_highlighter.hpp"
#include "base_text_buffer.hpp"
#include <cwctype>
#include <iostream>

static const char *keywords[] = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", 
    "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", 
    "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", 
    "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", 
    "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", 
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", 
    "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_assert", 
    "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", 
    "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", 
    "volatile", "wchar_t", "while", "xor", "xor_eq" };


CppHighlighter::CppHighlighter(BaseTextBuffer *textBuffer):
    textBuffer_(textBuffer),
    keywords_{ std::begin(keywords), std::end(keywords) }
{
    toFg_[Keyword] = Green;
    toBg_[Keyword] = White;

    toFg_[Ident] = Black;
    toBg_[Ident] = White;

    toFg_[Comment] = Brown;
    toBg_[Comment] = White;

    toFg_[Macro] = Violetred;
    toBg_[Macro] = White;

    toFg_[StringLiteral] = Steelblue;
    toBg_[StringLiteral] = White;

    toFg_[Number] = Blue;
    toBg_[Number] = White;

    toFg_[Other] = Black;
    toBg_[Other] = White;
}

Color CppHighlighter::fgColor(int x, int y) const
{
    if (y < static_cast<int>(types_.size()))
        if (x < static_cast<int>(types_[y].size()))
            return toFg_[types_[y][x]];
    return Black;
}

Color CppHighlighter::bgColor(int x, int y) const
{
    if (y < static_cast<int>(types_.size()))
        if (x < static_cast<int>(types_[y].size()))
            return toBg_[types_[y][x]];
    return White;
}

static bool isAllSpaces(std::wstring str)
{
    for (auto c: str)
        if (!iswspace(c))
            return false;
    return true;
}

std::pair<CppHighlighter::Type, int> CppHighlighter::getToken(int &x, int &y)
{
    int count = 0;
    Type type = Other;
    if (!outOfRange(x, y))
    {
        auto c = ch(x, y);
        if (iswalpha(c) || c == L'_')
        {
            std::string ident;
            while (!outOfRange(x, y))
            {
                auto c = ch(x, y);
                if (iswalnum(c) || c == L'_')
                    ident += c;
                else
                    break;
                ++count;
                moveForward(x, y);
            }
            if (keywords_.find(ident) != std::end(keywords_))
                type = Keyword;
            else
                type = Ident;
        } 
        else if (iswdigit(c))
        {
            while (!outOfRange(x, y))
            {
                auto c = ch(x, y);
                if (!iswxdigit(c))
                    break;
                ++count;
                moveForward(x, y);
            }
            type = Number;
        }
        else if ((x == 0 || isAllSpaces((*textBuffer_)[y].substr(0, x - 1))) && c == '#')
        {
            ++count;
            moveForward(x, y);
            while (!outOfRange(x, y))
            {
                auto c = ch(x, y);
                if (!iswalnum(c) && c != L'_')
                    break;
                ++count;
                moveForward(x, y);
            }
            type = Macro;
        }
        else  if (c == L'"' || c == L'\'')
        {
            wchar_t openChar = c;
            ++count;
            moveForward(x, y);
            while (!outOfRange(x, y))
            {
                auto c = ch(x, y);
                if (c == '\\')
                {
                    ++count;
                    moveForward(x, y);
                    c = ch(x, y);
                    if (iswdigit(c))
                    {
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                    }
                    else if (c == L'x')
                    {
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                    }
                    else if (c == L'u')
                    {
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                    }
                    else if (c == L'U')
                    {
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                        ++count;
                        moveForward(x, y);
                    }
                }
                else if (c == openChar)
                {
                    ++count;
                    moveForward(x, y);
                    break;
                }
                ++count;
                moveForward(x, y);
            }
            type = StringLiteral;
        }
        else
        {
            int tmpX = x;
            int tmpY = y;
            moveForward(tmpX, tmpY);
            auto c2 = ch(tmpX, tmpY);
            if (c == L'/' && c2 == L'/')
            {
                x = tmpX;
                y = tmpY;
                count = (*textBuffer_)[y].size() - x + 2;
                x = (*textBuffer_)[y].size();
                moveForward(x, y);
                type = Comment;
            }
            else if (c == L'/' && c2 == L'*')
            {
                x = tmpX;
                y = tmpY;
                while (!outOfRange(x, y))
                {
                    c = c2;
                    c2 = ch(x, y);
                    if (c == L'*' && c2 == L'/')
                    {
                        ++count;
                        moveForward(x, y);
                        break;
                    }
                    ++count;
                    moveForward(x, y);
                }
                type = Comment;
            }
            else 
            {
                type = Other;
                ++count;
                moveForward(x, y);
            }
        }
    }
    return std::make_pair(type, count);
}

void CppHighlighter::update(const Coord &start, const Coord &end)
{
    types_.resize(textBuffer_->size());
    for (int y = 0; y < textBuffer_->size(); ++y)
    {
        auto &line = (*textBuffer_)[y];
        types_[y].resize(line.size() + 1);
    }
    
    int x = start.x; 
    int y = start.y;
    auto t = types_[y][x];
    auto c = 0;
    while (!outOfRange(x, y))
    {
        int tmpX = x;
        int tmpY = y;
        moveBackward(tmpX, tmpY);
        if (outOfRange(tmpX, tmpY))
            break;
        if (t != types_[tmpY][tmpX])
        {
            t = types_[tmpY][tmpX];
            ++c;
            if (c > 2)
                break;
        }
        x = tmpX;
        y = tmpY;
    }
    while (!outOfRange(x, y))
    {
        std::pair<Type, int> token = getToken(x, y);
        int count = token.second;
        int tmpX = x;
        int tmpY = y;
        for (; count > 0; --count)
        {
            moveBackward(tmpX, tmpY);
            if (!outOfRange(tmpX, tmpY))
            {
                types_[tmpY][tmpX] = token.first;
            }
        }
        if (y > end.y || (y == end.y && x > end.x))
            break;
    }
}

void CppHighlighter::moveForward(int &x, int &y) const
{
    if (outOfRange(x, y))
        return;
    ++x;
    if (x > static_cast<int>((*textBuffer_)[y].size()))
    {
        x = 0;
        ++y;
    }
}

void CppHighlighter::moveBackward(int &x, int &y) const
{
    --x;
    if (x < 0)
    {
        --y;
        if (y >= 0)
            x = static_cast<int>((*textBuffer_)[y].size());
        else
            x = 0;
        if (outOfRange(x, y))
            return;
    }
}

bool CppHighlighter::outOfRange(int x, int y) const
{
    if (y < 0 || y >= static_cast<int>(textBuffer_->size()))
        return true;
    if (x < 0 || x > static_cast<int>((*textBuffer_)[y].size()))
        return true;
    return false;
}

 wchar_t CppHighlighter::ch(int x, int y) const
 {
     if (!outOfRange(x, y))
         return (*textBuffer_)[y][x];
     else
         return L'\0';
}
