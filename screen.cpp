#include "screen.hpp"
#include "base_text_buffer.hpp"
#include "painter.hpp"
#include "key_event.hpp"
#include "resize_event.hpp"
#include "text_input_event.hpp"
#include "SDL2/SDL_ttf.h"
#include <iostream>

Screen::Char::Char(wchar_t ach, Color afg, Color abg):
    ch(ach),
    fg(afg),
    bg(abg)
{}

Screen::Screen(Widget *parent):
    Widget(parent),
    cursor_{0, 0},
    hScroll_{0},
    vScroll_{0},
    textBuffer_{nullptr}
{
    Painter p(this);
    glyphWidth_ = p.glyphWidth();
    glyphHeight_ = p.glyphHeight();
    ResizeEvent e{width(), height()};
    resizeEvent(e);
}

void Screen::paintEvent(PaintEvent &)
{
    Painter p(this);
    for (size_t y = 0; y < ch_.size(); ++y)
        for (size_t x = 0; x < ch_[y].size(); ++x)
        {
            const auto &c = ch_[y][x];
            p.renderGlyph(c.ch, x * glyphWidth_, y * glyphHeight_, c.fg, c.bg);
        }
    p.setColor(Gray);
    p.drawLine(cursor_.x * glyphWidth_, cursor_.y * glyphHeight_,
               cursor_.x * glyphWidth_, (cursor_.y + 1) * glyphHeight_);
}

bool Screen::keyPressEvent(KeyEvent &e)
{
    if (!textBuffer_)
        return false;
    switch (e.modifiers()) 
    {
    case KeyEvent::MNone:
        switch (e.key())
        {
        case KeyEvent::KDelete:
            textBuffer_->del();
            textBuffer_->render(this);
            break;
        case KeyEvent::KBackspace:
            textBuffer_->backspace();
            textBuffer_->render(this);
            break;
        case KeyEvent::KReturn:
            textBuffer_->insert(L"\n");
            textBuffer_->render(this);
            break;
        case KeyEvent::KLeft:
            {
                auto cursor = textBuffer_->cursor();
                if (cursor.x > 0)
                    --cursor.x;
                else
                {
                    if (cursor.y > 0)
                    {
                        --cursor.y;
                        cursor.x = (*textBuffer_)[cursor.y].size();
                    }
                }
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KRight:
            {
                auto cursor = textBuffer_->cursor();
                if (cursor.x < static_cast<int>((*textBuffer_)[cursor.y].size()))
                    ++cursor.x;
                else
                {
                    if (cursor.y < textBuffer_->size() - 1)
                    {
                        ++cursor.y;
                        cursor.x = 0;
                    }
                }
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KUp:
            {
                auto cursor = textBuffer_->cursor();
                if (cursor.y > 0)
                {
                    --cursor.y;
                    if (cursor.x > static_cast<int>((*textBuffer_)[cursor.y].size()))
                        cursor.x = (*textBuffer_)[cursor.y].size();
                }
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KDown:
            {
                auto cursor = textBuffer_->cursor();
                if (cursor.y < textBuffer_->size() - 1)
                {
                    ++cursor.y;
                    if (cursor.x > static_cast<int>((*textBuffer_)[cursor.y].size()))
                        cursor.x = (*textBuffer_)[cursor.y].size();
                }
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KHome:
            {
                auto cursor = textBuffer_->cursor();
                cursor.x = 0;
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KEnd:
            {
                auto cursor = textBuffer_->cursor();
                cursor.x = (*textBuffer_)[cursor.y].size();
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KPageUp:
            {
                vScroll_ -= heightCh() - 1;
                if (vScroll_ < 0)
                    vScroll_ = 0;
                auto cursor = textBuffer_->cursor();
                cursor.y -= heightCh() - 1;
                if (cursor.y < vScroll_)
                    cursor.y = vScroll_;
                else if (cursor.y >= vScroll_ + heightCh())
                    cursor.y = vScroll_ + heightCh() - 1;
                if (cursor.y >= textBuffer_->size())
                    cursor.y = textBuffer_->size() - 1;
                if (cursor.y < 0)
                    cursor.y = 0;
                if (cursor.x > static_cast<int>((*textBuffer_)[cursor.y].size()))
                    cursor.x = (*textBuffer_)[cursor.y].size();
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KPageDown:
            {
                vScroll_ += heightCh() - 1;
                if (vScroll_ >= textBuffer_->size() - 1)
                    vScroll_ = textBuffer_->size() - 2;
                auto cursor = textBuffer_->cursor();
                cursor.y += heightCh() - 1;
                if (cursor.y < vScroll_)
                    cursor.y = vScroll_;
                else if (cursor.y >= vScroll_ + heightCh())
                    cursor.y = vScroll_ + heightCh() - 1;
                if (cursor.y >= textBuffer_->size())
                    cursor.y = textBuffer_->size() - 1;
                if (cursor.y < 0)
                    cursor.y = 0;
                if (cursor.x > static_cast<int>((*textBuffer_)[cursor.y].size()))
                    cursor.x = (*textBuffer_)[cursor.y].size();
                textBuffer_->setCursor(cursor);
                textBuffer_->render(this);
                break;
            }
        default:
            break;
        };
        break;
    case KeyEvent::MLCtrl:
    case KeyEvent::MRCtrl:
        switch (e.key())
        {
        case KeyEvent::KHome:
            {
                textBuffer_->setCursor(0, 0);
                textBuffer_->render(this);
                break;
            }
        case KeyEvent::KEnd:
            {
                textBuffer_->setCursor((*textBuffer_)[textBuffer_->size() - 1].size(), textBuffer_->size() - 1);
                textBuffer_->render(this);
                break;
            }
        default:
            break;
        }
        break;
    }
    
    return true;
}

bool Screen::textInputEvent(TextInputEvent &e)
{
    if (textBuffer_)
    {
        textBuffer_->insert(e.text());
        textBuffer_->render(this);
        return true;
    }
    return false;
}

void Screen::resizeEvent(ResizeEvent &e)
{
    ch_.resize(heightCh());
    for (auto &r: ch_)
        r.resize(widthCh());
}

int Screen::widthCh() const
{
    return width() / glyphWidth_;
}

int Screen::heightCh() const
{
    return height() / glyphHeight_;
}

Screen::Char &Screen::ch(int x, int y)
{
    return ch_[y][x];
}

const Screen::Char &Screen::ch(int x, int y) const
{
    return ch_[y][x];
}

Coord Screen::cursor() const
{
    return cursor_;
}

void Screen::setCursor(Coord value)
{
    if (value.y < 0)
    {
        vScroll_ += value.y;
        value.y = 0;
    }
    if (value.y > heightCh() - 1)
    {
        vScroll_ += value.y - heightCh() + 1;
        value.y = heightCh() - 1;
    }
    cursor_ = value;
}

void Screen::setCursor(int x, int y)
{
    setCursor(Coord{ x, y });
}

BaseTextBuffer *Screen::textBuffer() const
{
    return textBuffer_;
}

void Screen::setTextBuffer(BaseTextBuffer *value)
{
    if (value != textBuffer_)
    {
        textBuffer_ = value;
        if (textBuffer_)
            textBuffer_->render(this);
        else
        {
            for (size_t y = 0; y < ch_.size(); ++y)
                for (size_t x = 0; x < ch_[y].size(); ++x)
                    ch_[y][x] = '\0';
            update();
        }
    }
}

int Screen::hScroll() const
{
    return hScroll_;
}

void Screen::setHScroll(int value)
{
    hScroll_ = value;
    if (textBuffer_)
        textBuffer_->render(this);
}

int Screen::vScroll() const
{
    return vScroll_;
}

void Screen::setVScroll(int value)
{
    vScroll_ = value;
    if (textBuffer_)
        textBuffer_->render(this);
}
