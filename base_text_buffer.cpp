#include "base_text_buffer.hpp"
#include "screen.hpp"

BaseTextBuffer::BaseTextBuffer():
    isReadOnly_{false},
    cursor_{0, 0},
    highlighter_{nullptr}
{}

BaseTextBuffer::~BaseTextBuffer()
{
    delete highlighter_;
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
            Color fgColor = Black;
            Color bgColor = White;
            if (highlighter_)
            {
                fgColor = highlighter_->fgColor(xx, yy);
                bgColor = highlighter_->bgColor(xx, yy);
            }
            if (!screen->isSelected({ xx, yy }))
                screen->ch(x, y) = { xx < static_cast<int>(line.size()) ? line[xx] : L'\0', fgColor, bgColor };
            else
                screen->ch(x, y) = { xx < static_cast<int>(line.size()) ? line[xx] : L'\0', Black, Gray90 };
        }
    }
    screen->update();
}


std::wstring BaseTextBuffer::preInsert(Coord &cursor, std::wstring value)
{
    return value;
}

void BaseTextBuffer::insert(Coord &cursor, std::wstring value)
{
    value = preInsert(cursor, value);
    if (!value.empty())
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
    postInsert(cursor, value);
}

void BaseTextBuffer::postInsert(Coord &cursor, std::wstring value)
{
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
        if (highlighter_)
            highlighter_->update(cursor.x, cursor.y);
    }
}

int BaseTextBuffer::preDel(Coord &, int value)
{
    return value;
}

void BaseTextBuffer::del(Coord &cursor, int value)
{
    value = preDel(cursor, value);
    if (value > 0)
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
    postDel(cursor, value);
}

void BaseTextBuffer::postDel(Coord &, int)
{
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
        if (highlighter_)
            highlighter_->update(cursor.x, cursor.y);
    }
    return result;
}

int BaseTextBuffer::preBackspace(Coord &, int value)
{
    return value;
}

void BaseTextBuffer::backspace(Coord &cursor, int value)
{
    value = preBackspace(cursor, value);
    if (value > 0)
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
    postBackspace(cursor, value);
}

void BaseTextBuffer::postBackspace(Coord &, int)
{
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

std::wstring BaseTextBuffer::name() const
{
    return name_;
}

void BaseTextBuffer::setName(std::wstring value)
{
    name_ = value;
}

Coord BaseTextBuffer::cursor() const
{
    return cursor_;
}

void BaseTextBuffer::setCursor(Coord value)
{
    cursor_ = value;
}

bool BaseTextBuffer::isModified() const
{
    return undoStack_.isModified();
}

void BaseTextBuffer::clearModified()
{
    undoStack_.clearModified();
}
