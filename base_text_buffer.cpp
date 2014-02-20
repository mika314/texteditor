#include "base_text_buffer.hpp"
#include "screen.hpp"

BaseTextBuffer::BaseTextBuffer():
    cursor_{ 0, 0 }
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

void BaseTextBuffer::render(Screen *screen) const
{
    auto tmp = cursor();
    screen->setCursor(tmp.x - screen->hScroll(), tmp.y - screen->vScroll());
    for (int y = 0; y < screen->heightCh(); ++y)
    {
        const auto yy = y + screen->vScroll();
        const auto &line = yy < size() ? operator[](yy) : L"";
        for (int x = 0; x < screen->widthCh(); ++x)
        {
            const auto xx = x + screen->hScroll();
            screen->ch(x, y) = xx < static_cast<int>(line.size()) ? line[xx] : L'\0';
        }
    }
    screen->update();
}

void BaseTextBuffer::insert(std::wstring value)
{
    auto &line = buffer_[cursor_.y];
    for (wchar_t c: value)
    {
        if (c != L'\n')
        {
            line.insert(begin(line) + cursor_.x, c);
            ++cursor_.x;
        }
        else
        {
            std::wstring tmp { begin(line) + cursor_.x, end(line) };
            line.erase(begin(line) + cursor_.x, end(line));
            buffer_.insert(begin(buffer_) + cursor_.y + 1, tmp);
            ++cursor_.y;
            cursor_.x = 0;
        }
    }
}

Coord BaseTextBuffer::cursor() const
{
    return cursor_;
}

void BaseTextBuffer::setCursor(Coord value)
{
    cursor_ = value;
}

void BaseTextBuffer::setCursor(int x, int y)
{
    cursor_ = { x, y };
}

void BaseTextBuffer::del(int value)
{
    for (int i = 0; i < value; ++i)
    {
        auto &line = buffer_[cursor_.y];
        if (begin(line) + cursor_.x != end(line))
            line.erase(begin(line) + cursor_.x);
        else
        {
            if (cursor_.y < static_cast<int>(buffer_.size()))
            {
                auto tmp = buffer_[cursor_.y + 1];
                buffer_.erase(begin(buffer_) + cursor_.y + 1);
                buffer_[cursor_.y] += tmp;
            }
        }
    }
}

void BaseTextBuffer::backspace(int value)
{
    for (int i = 0; i < value; ++i)
    {
        if (cursor_.x > 0)
            --cursor_.x;
        else
        {
            if (cursor_.y > 0)
            {
                --cursor_.y;
                cursor_.x = buffer_[cursor_.y].size();
            }
            else
                return;
        }
        del();
    }
}
