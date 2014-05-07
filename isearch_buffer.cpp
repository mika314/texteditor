#include "isearch_buffer.hpp"
#include "screen.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

IsearchBuffer::IsearchBuffer(Screen *screen, bool fsearch):
    screen_(screen),
    initialCursor_(screen_->cursor()),
    fsearch_(fsearch)
{
    buffer_.push_back(L"I-search:");
}

void IsearchBuffer::postInsert(Coord &cursor, std::wstring value)
{
    assert(screen_->textBuffer());
    std::transform(begin(value), end(value), begin(value), ::tolower);
    searchString_ += value;
    search();
}

int IsearchBuffer::preBackspace(Coord &cursor, int value)
{
    if (searchString_.size() > 0)
        return value;
    else
        return 0;
}

void IsearchBuffer::postBackspace(Coord &cursor, int value)
{
    if (searchString_.size() > 0)
    {
        searchString_.resize(searchString_.size() - value);
        screen_->setCursor(initialCursor_);
        search();
    }
}

void IsearchBuffer::findNext()
{
    fsearch_ = true;
    if (searchString_.empty())
        return;
    screen_->moveCursorRight();
    if (!search())
    {
        screen_->setCursor(Coord{ 0, 0 });
        search();
    }
}

void IsearchBuffer::findPrev()
{
    fsearch_ = false;
    if (searchString_.empty())
        return;
    for (size_t i = 0; i <= searchString_.size(); ++i)
        screen_->moveCursorLeft();
    if (!search())
    {
        screen_->setCursor(Coord{ 
                static_cast<int>((*screen_->textBuffer())[screen_->textBuffer()->size() - 1].size()),
                    screen_->textBuffer()->size() - 1 });
        search();
    }
}

bool IsearchBuffer::search()
{
    if (fsearch_)
        return fsearch();
    else
        return rsearch();
}

bool IsearchBuffer::fsearch()
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
            return true;
        }
        ++y;
        x = 0;
    }
    return false;
}

bool IsearchBuffer::rsearch()
{
    size_t x = std::max(0, static_cast<int>(screen_->cursor().x));
    int y = screen_->cursor().y;
    while (y >= 0)
    {
        std::cout << "y: " << y << " x: " << x << std::endl;
        auto tmpStr = (*screen_->textBuffer())[y];
        std::wcout << tmpStr << std::endl;
        std::transform(begin(tmpStr), end(tmpStr), begin(tmpStr), ::tolower);
        auto tmp = tmpStr.rfind(searchString_, x);
        if (tmp != std::wstring::npos)
        {
            screen_->setCursor(tmp + searchString_.size(), y);
            screen_->setStartSelection(Coord{ static_cast<int>(tmp), y });
            screen_->setEndSelection(Coord{ static_cast<int>(tmp + searchString_.size()), y });
            return true;
        }
        --y;
        if (y >= 0)
            x = (*screen_->textBuffer())[y].size();
    }
    return false;
}
