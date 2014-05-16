#include "text_screen.hpp"
#include "status_bar.hpp"
#include "isearch_buffer.hpp"
#include "base_text_buffer.hpp"
#include "painter.hpp"
#include "key_event.hpp"
#include "resize_event.hpp"
#include "text_input_event.hpp"
#include "to_utf16.hpp"
#include "to_utf8.hpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>

TextScreen::Char::Char(wchar_t ach, Color afg, Color abg):
    ch(ach),
    fg(afg),
    bg(abg)
{}

bool TextScreen::Char::operator!=(const Char &value) const
{
    return ch != value.ch || fg != value.fg || bg != value.bg;
}


TextScreen::TextScreen(Widget *parent):
    Widget(parent),
    cursor_{0, 0},
    startSelection_{-1, -1},
    endSelection_{-1, -1},
    hScroll_{0},
    vScroll_{0},
    textBuffer_{nullptr},
    statusBar_{nullptr},
    prevX_{-1},
    prevY_{-1}
{
    Painter p(this);
    glyphWidth_ = p.glyphWidth();
    glyphHeight_ = p.glyphHeight();
    ResizeEvent e{width(), height()};
    resizeEvent(e);
}

TextScreen::~TextScreen()
{
    if (statusBar_)
        delete statusBar_->textBuffer();
}

void TextScreen::paintEvent(PaintEvent &)
{
    Painter p(this);
    for (size_t y = 0; y < ch_.size(); ++y)
        for (size_t x = 0; x < ch_[y].size(); ++x)
        {
            const auto &prevC = prevCh_[y][x];
            const auto &c = ch_[y][x];
            if (prevC != c || (prevX_ == static_cast<int>(x) && prevY_ == static_cast<int>(y)))
                p.renderGlyph(c.ch, x * glyphWidth_, y * glyphHeight_, c.fg, c.bg);
        }
    auto xx = cursor_.x - hScroll_;
    auto yy = cursor_.y - vScroll_;
    if (hasFocus())
        p.setColor(Red);
    else
        p.setColor(Gray);
    p.drawLine(xx * glyphWidth_, yy * glyphHeight_,
               xx * glyphWidth_, (yy + 1) * glyphHeight_ - 1);
    p.drawLine(xx * glyphWidth_ + 1, yy * glyphHeight_,
               xx * glyphWidth_ + 1, (yy + 1) * glyphHeight_ - 1);
    p.setColor(Gray90);
    p.drawLine(0, height() - 1, width() - 1, height() - 1);
    p.drawLine(width() - 1, 0, width() - 1, height() - 1);
    if (textBuffer_)
    {
        int sbHeight = std::max(3, 
                                textBuffer_->size() != 0 ? 
                                heightCh() * height() / textBuffer_->size() : 
                                heightCh() * height());
        int sbTop = textBuffer_->size() != 0 ? 
            vScroll() * height() / textBuffer_->size() :
            0;
        p.setColor(Blue);
        p.drawLine(width() - 1, sbTop, width() - 1, sbTop + sbHeight);
    }
    prevX_ = xx;
    prevY_ = yy;
    prevCh_ = ch_;
}

bool TextScreen::keyPressEvent(KeyEvent &e)
{
    if (!textBuffer_)
        return false;
    bool result = true;
    switch (e.modifiers()) 
    {
    case KeyEvent::MNone:
        switch (e.key())
        {
        case KeyEvent::KEscape:
            escStatusBar();
            break;
        case KeyEvent::KDelete:
            escStatusBar();
            if (deleteSelected() == 0)
            {
                textBuffer_->del(cursor_);
                setCursor(cursor_);
                render();
            }
            break;
        case KeyEvent::KBackspace:
            if (!statusBar_ || !statusBar_->textBuffer())
            {
                if (deleteSelected() == 0)
                {
                    textBuffer_->backspace(cursor_);
                    setCursor(cursor_);
                    render();
                }
            }
            else
            {
                statusBar_->keyPressEvent(e);
                render();
            }
            break;
        case KeyEvent::KReturn:
            {
                deleteSelected();
                escStatusBar();
                textBuffer_->insert(cursor_, L"\n");
                setCursor(cursor_);
                render();
                break;
            }
        case KeyEvent::KLeft:
            moveCursor(&TextScreen::moveCursorLeft);
            break;
        case KeyEvent::KRight:
            moveCursor(&TextScreen::moveCursorRight);
            break;
        case KeyEvent::KUp:
            moveCursor(&TextScreen::moveCursorUp);
            break;
        case KeyEvent::KDown:
            moveCursor(&TextScreen::moveCursorDown);
            break;
        case KeyEvent::KHome:
            moveCursor(&TextScreen::moveCursorHome);
            break;
        case KeyEvent::KEnd:
            moveCursor(&TextScreen::moveCursorEnd);
            break;
        case KeyEvent::KPageUp:
            moveCursor(&TextScreen::moveCursorPageUp);
            break;
        case KeyEvent::KPageDown:
            moveCursor(&TextScreen::moveCursorPageDown);
            break;
        default:
            result = false;
            break;
        };
        break;
    case KeyEvent::MLCtrl:
    case KeyEvent::MRCtrl:
        switch (e.key())
        {
        case KeyEvent::KHome:
            setCursor(0, 0);
            render();
            break;
        case KeyEvent::KEnd:
            setCursor((*textBuffer_)[textBuffer_->size() - 1].size(), textBuffer_->size() - 1);
            render();
            break;
        case KeyEvent::KV:
            paste();
            render();
            break;
        case KeyEvent::KInsert:
        case KeyEvent::KC:
            copy();
            render();
            break;
        case KeyEvent::KX:
            cut();
            render();
            break;
        case KeyEvent::KA:
            selectAll();
            render();
        case KeyEvent::KF:
            startIsearch();
            break;
        case KeyEvent::KZ:
            {
                Coord c = cursor();
                textBuffer_->undo(c);
                setCursor(c);
                render();
                break;
            }
        case KeyEvent::KR:
            {
                Coord c = cursor();
                textBuffer_->redo(c);
                setCursor(c);
                render();
                break;
            }
        default:
            result = false;
            break;
        }
        break;
    case KeyEvent::MLShift:
    case KeyEvent::MRShift:
        switch (e.key())
        {
        case KeyEvent::KLeft:
            select(&TextScreen::moveCursorLeft);
            break;
        case KeyEvent::KRight:
            select(&TextScreen::moveCursorRight);
            break;
        case KeyEvent::KUp:
            select(&TextScreen::moveCursorUp);
            break;
        case KeyEvent::KDown:
            select(&TextScreen::moveCursorDown);
            break;
        case KeyEvent::KHome:
            select(&TextScreen::moveCursorHome);
            break;
        case KeyEvent::KEnd:
            select(&TextScreen::moveCursorEnd);
            break;
        case KeyEvent::KPageUp:
            select(&TextScreen::moveCursorPageUp);
            break;
        case KeyEvent::KPageDown:
            select(&TextScreen::moveCursorPageDown);
            break;
        case KeyEvent::KInsert:
            paste();
            render();
            break;
        case KeyEvent::KDelete:
            cut();
            render();
            break;
        default:
            result = false;
            break;
        };
        break;
    default:
        result = false;
        break;
    }
    
    return result;
}

bool TextScreen::textInputEvent(TextInputEvent &e)
{
    if (textBuffer_)
    {
        if (!statusBar_ || !statusBar_->textBuffer())
        {
            deleteSelected();
            textBuffer_->insert(cursor_, e.text());
            setCursor(cursor_);
            render();
        }
        else
        {
            statusBar_->textInputEvent(e);
            render();
        }
        return true;
    }
    return false;
}

void TextScreen::resizeEvent(ResizeEvent &e)
{
    ch_.resize(heightCh());
    for (auto &r: ch_)
    {
        r.resize(widthCh());
    }
    prevCh_.resize(heightCh());
    for (auto &r: prevCh_)
    {
        r.resize(widthCh());
        for (auto &i: r)
            i.ch = 0xffff;
    }
    render();
}

int TextScreen::widthCh() const
{
    return (width() + glyphWidth_ - 1) / glyphWidth_;
}

int TextScreen::heightCh() const
{
    return (height() + glyphHeight_ - 1) / glyphHeight_;
}

TextScreen::Char &TextScreen::ch(int x, int y)
{
    return ch_[y][x];
}

const TextScreen::Char &TextScreen::ch(int x, int y) const
{
    return ch_[y][x];
}

Coord TextScreen::cursor() const
{
    return cursor_;
}

void TextScreen::setCursor(Coord value)
{
    if (value.y < vScroll_)
    {
        vScroll_ = value.y;
        value.y = vScroll_;
    }
    if (value.y > std::max(0, vScroll_ + heightCh() - 2))
    {
        vScroll_ = value.y - heightCh() + 2;
        value.y = vScroll_ + heightCh() - 2;
    }
    cursor_ = value;
}

void TextScreen::setCursor(int x, int y)
{
    setCursor(Coord{ x, y });
}

BaseTextBuffer *TextScreen::textBuffer() const
{
    return textBuffer_;
}

void TextScreen::setTextBuffer(BaseTextBuffer *value)
{
    if (value != textBuffer_)
    {
        setStartSelection(Coord{-1, -1});
        setEndSelection(Coord{-1, -1});
        
        if (textBuffer_)
            textBuffer_->setCursor(cursor());
        textBuffer_ = value;
        if (value)
            setCursor(value->cursor());
        else
            setCursor(0, 0);
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

int TextScreen::hScroll() const
{
    return hScroll_;
}

void TextScreen::setHScroll(int value)
{
    hScroll_ = value;
    render();
}

int TextScreen::vScroll() const
{
    return vScroll_;
}

void TextScreen::setVScroll(int value)
{
    vScroll_ = value;
    render();
}

Coord TextScreen::startSelection() const
{
    return startSelection_;
}

void TextScreen::setStartSelection(Coord value)
{
    startSelection_ = value;
}

Coord TextScreen::endSelection() const
{
    return endSelection_;
}

void TextScreen::setEndSelection(Coord value)
{
    endSelection_ = value;
}

bool TextScreen::isSelected(Coord value) const
{
    Coord s, e;
    if (startSelection().y > endSelection().y ||
        (startSelection().y == endSelection().y && startSelection().x > endSelection().x))
    {
        s = endSelection();
        e = startSelection();
    }
    else
    {
        s = startSelection();
        e = endSelection();
    }

    if (s.x == -1 ||
        s.y == -1 ||
        e.x == -1 ||
        e.y == -1)
        return false;
    return (value.y > s.y && value.y < e.y) ||
        (value.y == s.y && value.y != e.y && value.x >= s.x) ||
        (value.y == e.y && value.y != s.y && value.x < e.x) ||
        (value.y == s.y && value.y != e.y && value.x >= s.x) ||
        (value.y == e.y && value.y == s.y && value.x < e.x && value.x >= s.x);
}

StatusBar *TextScreen::statusBar() const
{
    return statusBar_;
}
void TextScreen::setStatusBar(StatusBar *value)
{
    statusBar_ = value;
}


int TextScreen::glyphHeight() const
{
    return glyphHeight_;
}

int TextScreen::glyphWidth() const
{
    return glyphWidth_;
}


void TextScreen::moveCursorLeft()
{
    if (cursor_.x > 0)
        --cursor_.x;
    else
    {
        if (cursor_.y > 0)
        {
            --cursor_.y;
            cursor_.x = (*textBuffer_)[cursor_.y].size();
        }
    }
    setCursor(cursor_);
}

void TextScreen::moveCursorRight()
{
    if (cursor_.x < static_cast<int>((*textBuffer_)[cursor_.y].size()))
        ++cursor_.x;
    else
    {
        if (cursor_.y < textBuffer_->size() - 1)
        {
            ++cursor_.y;
            cursor_.x = 0;
        }
    }
    setCursor(cursor_);
}

void TextScreen::moveCursorUp()
{
    if (cursor_.y > 0)
    {
        --cursor_.y;
        if (cursor_.x > static_cast<int>((*textBuffer_)[cursor_.y].size()))
            cursor_.x = (*textBuffer_)[cursor_.y].size();
    }
    setCursor(cursor_);
}

void TextScreen::moveCursorDown()
{
    if (cursor_.y < textBuffer_->size() - 1)
    {
        ++cursor_.y;
        if (cursor_.x > static_cast<int>((*textBuffer_)[cursor_.y].size()))
            cursor_.x = (*textBuffer_)[cursor_.y].size();
    }
    setCursor(cursor_);
}

void TextScreen::moveCursorHome()
{
    cursor_.x = 0;
    setCursor(cursor_);
}

void TextScreen::moveCursorEnd()
{
    cursor_.x = (*textBuffer_)[cursor_.y].size();
    setCursor(cursor_);
}

void TextScreen::moveCursorPageUp()
{
    vScroll_ -= heightCh() - 1;
    if (vScroll_ < 0)
        vScroll_ = 0;
    cursor_.y -= heightCh() - 1;
    if (cursor_.y < vScroll_)
        cursor_.y = vScroll_;
    else if (cursor_.y >= vScroll_ + heightCh())
        cursor_.y = vScroll_ + heightCh() - 1;
    if (cursor_.y >= textBuffer_->size())
        cursor_.y = textBuffer_->size() - 1;
    if (cursor_.y < 0)
        cursor_.y = 0;
    if (cursor_.x > static_cast<int>((*textBuffer_)[cursor_.y].size()))
        cursor_.x = (*textBuffer_)[cursor_.y].size();
    setCursor(cursor_);
}

void TextScreen::moveCursorPageDown()
{
    vScroll_ += heightCh() - 1;
    if (vScroll_ >= textBuffer_->size() - 1)
        vScroll_ = textBuffer_->size() - 2;
    cursor_.y += heightCh() - 1;
    if (cursor_.y < vScroll_)
        cursor_.y = vScroll_;
    else if (cursor_.y >= vScroll_ + heightCh())
        cursor_.y = vScroll_ + heightCh() - 1;
    if (cursor_.y >= textBuffer_->size())
        cursor_.y = textBuffer_->size() - 1;
    if (cursor_.y < 0)
        cursor_.y = 0;
    if (cursor_.x > static_cast<int>((*textBuffer_)[cursor_.y].size()))
        cursor_.x = (*textBuffer_)[cursor_.y].size();
    setCursor(cursor_);
}

void TextScreen::select(void (TextScreen::*moveCursor)())
{
    if (startSelection().x == -1)
        setStartSelection(cursor());
    (this->*moveCursor)();
    setEndSelection(cursor());
    render();
}

void TextScreen::moveCursor(void (TextScreen::*moveCursorFunc)())
{
    escStatusBar();
    (this->*moveCursorFunc)();
    setStartSelection({-1, -1});
    setEndSelection({-1, -1});
    render();
}

std::wstring TextScreen::getSelected() const
{
    if (startSelection().y == endSelection().y)
    {
        auto s = std::min(startSelection().x, endSelection().x);
        auto e = std::max(startSelection().x, endSelection().x);
        const auto &line = (*textBuffer_)[startSelection().y];
        std::wstring tmp{ begin(line) + s, begin(line) + e };
        return tmp;
    }
    else
    {
        Coord s, e;
        if (startSelection().y > endSelection().y ||
            (startSelection().y == endSelection().y && startSelection().x > endSelection().x))
        {
            s = endSelection();
            e = startSelection();
        }
        else
        {
            s = startSelection();
            e = endSelection();
        }
        std::wstring tmp;
        const auto &firstLine = (*textBuffer_)[s.y] + L'\n';
        tmp += { begin(firstLine) + s.x, end(firstLine) };
        for (int y = s.y + 1; y <= e.y - 1; ++y)
            tmp += (*textBuffer_)[y] + L'\n';
        const auto &lastLine = (*textBuffer_)[e.y];
        tmp += { begin(lastLine), begin(lastLine) + e.x };
        return tmp;
    }
}


int TextScreen::copy()
{
    std::wstring tmp = getSelected();
    if (!tmp.empty())
        SDL_SetClipboardText(toUtf8(tmp).c_str());
    return tmp.size();
}

void TextScreen::paste()
{
    setStartSelection(cursor());
    textBuffer_->insert(cursor_, toUtf16(SDL_GetClipboardText()));
    setStartSelection({-1, -1});
    setEndSelection({-1, -1});
}

void TextScreen::cut()
{
    if (startSelection().y > endSelection().y ||
        (startSelection().y == endSelection().y && startSelection().x > endSelection().x))
        textBuffer_->del(cursor_, copy());
    else
        textBuffer_->backspace(cursor_, copy());
    setStartSelection({-1, -1});
    setEndSelection({-1, -1});
}

void TextScreen::selectAll()
{
    setStartSelection({0, 0});
    setEndSelection({static_cast<int>((*textBuffer_)[textBuffer_->size() - 1].size()), 
                static_cast<int>(textBuffer_->size() - 1)});
    setCursor(endSelection());
}

void TextScreen::startIsearch()
{
    if (!statusBar_)
    {
        if (auto *buffer = dynamic_cast<IsearchBuffer *>(textBuffer_))
            buffer->findNext();
    }
    else
    {
        if (!statusBar_->textBuffer())
        {
            statusBar_->setTextBuffer(new IsearchBuffer(this));
            statusBar_->moveCursorEnd();
        }
        else
        {
            statusBar_->startIsearch();
            render();
        }
    }
}

void TextScreen::escStatusBar()
{
    if (!statusBar_)
        return;
    if (statusBar_->textBuffer())
    {
        delete statusBar_->textBuffer();
        statusBar_->setTextBuffer(nullptr);
    }
}

void TextScreen::render()
{
    if (textBuffer_)
        textBuffer_->render(this);
}

int TextScreen::deleteSelected()
{
    if (startSelection() == Coord{-1, -1} ||
        endSelection() == Coord{-1, -1})
        return 0;
    int result = getSelected().size();
    if (startSelection().y > endSelection().y ||
        (startSelection().y == endSelection().y && startSelection().x > endSelection().x))
        textBuffer_->del(cursor_, result);
    else
        textBuffer_->backspace(cursor_, result);

    setStartSelection(Coord{-1, -1});
    setEndSelection(Coord{-1, -1});
    render();

    return result;
}
