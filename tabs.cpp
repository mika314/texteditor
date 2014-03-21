#include "tabs.hpp"
#include "painter.hpp"
#include "base_text_buffer.hpp"
#include "screen.hpp"

Tabs::Tabs(Widget *parent, Screen *screen):
    Widget(parent),
    screen_(screen),
    activeBuffer_(nullptr)
{}

void Tabs::addTextBuffer(BaseTextBuffer *textBuffer)
{
    textBuffersList_.push_back(textBuffer);
    setActiveBuffer(textBuffer);
}

std::vector<BaseTextBuffer *> &Tabs::textBuffersList()
{
    return textBuffersList_;
}

const std::vector<BaseTextBuffer *> &Tabs::textBuffersList() const
{
    return textBuffersList_;
}

void Tabs::setActiveBuffer(BaseTextBuffer *value)
{
    activeBuffer_ = value;
    if (screen_)
        screen_->setTextBuffer(value);
    update();
}

const BaseTextBuffer *Tabs::activeBuffer() const
{
    return activeBuffer_;
}

BaseTextBuffer *Tabs::activeBuffer()
{
    return activeBuffer_;
}

int Tabs::maxHeight() const
{
    return 20;
}

int Tabs::minHeight() const
{
    return 20;
}

void Tabs::paintEvent(PaintEvent &)
{
    Painter p(this);
    p.setColor(Gray);
    p.drawRect(0, 0, width(), height());
    int x = 0;
    for (auto buff: textBuffersList_)
    {
        auto w = buff->name().size() * p.glyphWidth();
        auto bgColor = activeBuffer_ == buff ? Gray76 : Gray40;
        p.setColor(bgColor);
        p.drawRect(x, 0, w + p.glyphWidth() * 5 / 2, height());
        int chP = x + p.glyphWidth() / 2;
        for (auto ch: buff->name())
        {
            p.renderGlyph(ch, chP, (maxHeight() - p.glyphHeight()) / 2, Black, bgColor);
            chP += p.glyphWidth();
        }
        chP += p.glyphWidth() / 2;
        p.renderGlyph(L'X', chP, (maxHeight() - p.glyphHeight()) / 2, Red, bgColor);

        x += w + p.glyphWidth() * 5 / 2;
    }
}
