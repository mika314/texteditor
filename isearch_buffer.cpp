#include "isearch_buffer.hpp"
#include "screen.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

IsearchBuffer::IsearchBuffer(Screen *screen):
    screen_(screen),
    initialCursor_(screen_->cursor())
{
    buffer_.push_back(L"I-search:");
}

void IsearchBuffer::insert(Coord &cursor, std::wstring value)
{
    BaseTextBuffer::insert(cursor, value);
    assert(screen_->textBuffer());
    std::transform(begin(value), end(value), begin(value), ::tolower);
    searchString_ += value;
    search();
}

void IsearchBuffer::backspace(Coord &cursor, int value)
{
    if (searchString_.size() > 0)
    {
        BaseTextBuffer::backspace(cursor, value);
        searchString_.resize(searchString_.size() - value);
        screen_->setCursor(initialCursor_);
        search();
    }
}

void IsearchBuffer::search()
{
    size_t x = std::max(0, static_cast<int>(screen_->cursor().x - searchString_.size()));
    int y = screen_->cursor().y;
    while (y < screen_->textBuffer()->size())
    {
        auto tmpStr = (*screen_->textBuffer())[y];
        std::transform(begin(tmpStr), end(tmpStr), begin(tmpStr), ::tolower);
        auto tmp = tmpStr.find(searchString_, x);
        if (tmp != std::wstring::npos)
        {
            screen_->setCursor(tmp + searchString_.size(), y);
            screen_->setStartSelection(Coord{ static_cast<int>(tmp), y });
            screen_->setEndSelection(Coord{ static_cast<int>(tmp + searchString_.size()), y });
            break;
        }
        ++y;
        x = 0;
    }
}
