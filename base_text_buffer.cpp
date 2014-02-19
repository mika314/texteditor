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
    auto tmp = cursor();
    screen->setCursor(tmp.x - screen->hScroll(), tmp.y - screen->vScroll());
    screen->update();
}

void BaseTextBuffer::insert(std::wstring value)
{
    auto &line = buffer_[cursor_.y];
    line.insert(begin(line) + cursor_.x, begin(value), end(value));
    cursor_.x += value.size();
}

Coord BaseTextBuffer::cursor() const
{
    return cursor_;
}

void BaseTextBuffer::setCursor(Coord value)
{
    cursor_ = value;
}

void BaseTextBuffer:: setCursor(int x, int y)
{
    cursor_ = { x, y };
}
