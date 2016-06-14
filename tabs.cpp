#include "tabs.hpp"
#include "painter.hpp"
#include "base_text_buffer.hpp"
#include "screen.hpp"
#include "application.hpp"
#include <algorithm>

Tabs::Tabs(Widget *parent):
  Widget(parent),
  activeTextBuffer_(nullptr)
{}

void Tabs::addTextBuffer(BaseTextBuffer *textBuffer)
{
  textBuffersList_.push_back(textBuffer);
  setActiveTextBuffer(textBuffer);
}

void Tabs::closeTextBuffer(BaseTextBuffer *textBuffer)
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_))
    return;
  textBuffersList_.erase(iter);
  Application::instance()->queueDelete(textBuffer);
}

void Tabs::closeActiveTextBuffer()
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_))
    return;
  deleteTextBuffer(activeTextBuffer_);
  Application::instance()->queueDelete(activeTextBuffer_);
  iter = textBuffersList_.erase(iter);
    
  if (iter != end(textBuffersList_))
    setActiveTextBuffer(*iter);
  else if (!textBuffersList_.empty())
    setActiveTextBuffer(textBuffersList_.back());
  else
    setActiveTextBuffer(nullptr);
}

void Tabs::switchToNextTextBuffer()
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_))
    return;
  ++iter;
  if (iter == end(textBuffersList_))
    return;
  setActiveTextBuffer(*iter);
}

void Tabs::switchToPrevTextBuffer()
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_))
    return;
  if (*iter == textBuffersList_.front())
    return;
  --iter;
  setActiveTextBuffer(*iter);
}

void Tabs::moveTextBufferLeft()
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_))
    return;
  if (*iter == textBuffersList_.front())
    return;
  std::swap(*iter, *(iter - 1));
  update();
}

void Tabs::moveTextBufferRight()
{
  auto iter = find(begin(textBuffersList_), end(textBuffersList_), activeTextBuffer_);
  if (iter == end(textBuffersList_) || iter + 1 == end(textBuffersList_))
    return;
  std::swap(*iter, *(iter + 1));
  update();
}

std::vector<BaseTextBuffer *> &Tabs::textBuffersList()
{
  return textBuffersList_;
}

const std::vector<BaseTextBuffer *> &Tabs::textBuffersList() const
{
  return textBuffersList_;
}

void Tabs::setActiveTextBuffer(BaseTextBuffer *value)
{
  activeTextBuffer_ = value;
  setTextBuffer(value);
  update();
}

const BaseTextBuffer *Tabs::activeTextBuffer() const
{
  return activeTextBuffer_;
}

BaseTextBuffer *Tabs::activeTextBuffer()
{
  return activeTextBuffer_;
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
    auto tabName = (buff->isModified() && !buff->isReadOnly() ? L"*" : L"") + buff->name();
    auto w = tabName.size() * p.glyphWidth();
    auto bgColor = activeTextBuffer_ == buff ? Gray76 : Gray40;
    p.setColor(bgColor);
    p.drawRect(x, 0, w + p.glyphWidth() * 5 / 2, height());
    int chP = x + p.glyphWidth() / 2;
    for (auto ch: tabName)
    {
      p.renderGlyph(ch, chP, (maxHeight() - p.glyphHeight()) / 2, Black, bgColor);
      chP += p.glyphWidth();
    }
    chP += p.glyphWidth() / 2;
    p.renderGlyph(L'X', chP, (maxHeight() - p.glyphHeight()) / 2, Red, bgColor);

    x += w + p.glyphWidth() * 5 / 2;
  }
}

