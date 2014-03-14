#include "base_text_buffer.hpp"
#include "screen.hpp"

BaseTextBuffer::BaseTextBuffer():
    isReadOnly_(false)
{}

BaseTextBuffer::~BaseTextBuffer()
{
}

const std::wstring &BaseTextBuffer::operator[](int line) const
{
    return buffer_[line];
}

std::wstring &BaseTextBuffer::operator[](int line)
{
    return buffer_[line];
}

int BaseTextBuffer::size() const
{
    return buffer_.size();
}

void BaseTextBuffer::undo(Coord &cursor)
{
    undoStack_.undo(cursor);
}

void BaseTextBuffer::redo(Coord &cursor)
{
    undoStack_.redo(cursor);
}


void BaseTextBuffer::render(Screen *screen) const
{
    for (int y = 0; y < screen->heightCh(); ++y)
    {
        const auto yy = y + screen->vScroll();
        const auto &line = yy < size() ? operator[](yy) : L"";
        for (int x = 0; x < screen->widthCh(); ++x)
        {
            const auto xx = x + screen->hScroll();
            if (!screen->isSelected({ xx, yy }))
                screen->ch(x, y) = xx < static_cast<int>(line.size()) ? line[xx] : L'\0';
            else
                screen->ch(x, y) = { xx < static_cast<int>(line.size()) ? line[xx] : L'\0', Black, Gray90 };
        }
    }
    screen->update();
}

void BaseTextBuffer::insert(Coord &cursor, std::wstring value)
{
    undoStack_.push(cursor, 
                    [value, this](Coord &c) -> int
                    { 
                        internalInsert(c, value);
                        return value.size();
                    },
                    [this](Coord &c, int size)
                    {
                        internalDelete(c, size);
                    });
}

void BaseTextBuffer::internalInsert(Coord &cursor, std::wstring value)
{
    if (isReadOnly())
        return;
    for (wchar_t c: value)
    {
        auto &line = buffer_[cursor.y];
        if (c != L'\n')
        {
            line.insert(begin(line) + cursor.x, c);
            ++cursor.x;
        }
        else
        {
            std::wstring tmp { begin(line) + cursor.x, end(line) };
            line.erase(begin(line) + cursor.x, end(line));
            buffer_.insert(begin(buffer_) + cursor.y + 1, tmp);
            ++cursor.y;
            cursor.x = 0;
        }
    }
}

void BaseTextBuffer::del(Coord &cursor, int value)
{
    undoStack_.push(cursor, 
                    [value, this](Coord &c) -> std::wstring
                    { 
                        return internalDelete(c, value); 
                    },
                    [this](Coord &c, const std::wstring &str)
                    {
                        Coord tmp = c;
                        internalInsert(c, str);
                        c = tmp;
                    });
}

std::wstring BaseTextBuffer::internalDelete(const Coord cursor, int value)
{
    if (isReadOnly())
        return L"";
    std::wstring result;
    for (int i = 0; i < value; ++i)
    {
        auto &line = buffer_[cursor.y];
        if (begin(line) + cursor.x != end(line))
        {
            result += line[cursor.x];
            line.erase(begin(line) + cursor.x);
        }
        else
        {
            if (cursor.y < static_cast<int>(buffer_.size()) - 1)
            {
                result += L"\n";
                auto tmp = buffer_[cursor.y + 1];
                buffer_.erase(begin(buffer_) + cursor.y + 1);
                buffer_[cursor.y] += tmp;
            }
        }
    }
    return result;
}

void BaseTextBuffer::backspace(Coord &cursor, int value)
{
    undoStack_.push(cursor, 
                    [value, this](Coord &c) -> std::pair<std::wstring, Coord>
                    { 
                        auto result = internalBackspace(c, value);
                        return std::make_pair(result, c);
                    },
                    [this](Coord &c, const std::pair<std::wstring, Coord> &s)
                    {
                        Coord ccc = s.second;
                        internalInsert(ccc, s.first);
                        c = ccc;
                    });
}

std::wstring BaseTextBuffer::internalBackspace(Coord &cursor, int value)
{
    if (isReadOnly())
        return L"";
    std::wstring result;
    for (int i = 0; i < value; ++i)
    {
        if (cursor.x > 0)
            --cursor.x;
        else
        {
            if (cursor.y > 0)
            {
                --cursor.y;
                cursor.x = buffer_[cursor.y].size();
            }
            else
                return L"";
        }
        result += internalDelete(cursor);
    }
    
    return { result.rbegin(), result.rend() };
}

bool BaseTextBuffer::isReadOnly() const
{
    return isReadOnly_;
}

void BaseTextBuffer::setReadOnly(bool value)
{
    isReadOnly_ = value;
}
