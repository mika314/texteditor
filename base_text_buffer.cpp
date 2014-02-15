#include "base_text_buffer.hpp"
#include "screen.hpp"

const std::wstring &BaseTextBuffer::operator[](int line) const
{
    tmpLine_ = std::to_wstring(line) + L" Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam";
    return tmpLine_;
}

std::wstring &BaseTextBuffer::operator[](int line)
{
    tmpLine_ = std::to_wstring(line) + L" Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam";
    return tmpLine_;
}

int BaseTextBuffer::size() const
{
    return 10000;
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
    screen->update();
}
