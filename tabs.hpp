#pragma once
#include "widget.hpp"
#include "signal_slot.hpp"
#include <vector>

class BaseTextBuffer;
class Screen;

class Tabs: public Widget
{
public:
    Tabs(Widget *parent);
    void addTextBuffer(BaseTextBuffer *);
    void closeTextBuffer(BaseTextBuffer *);
    void closeActiveTextBuffer();
    void switchToNextTextBuffer();
    void switchToPrevTextBuffer();
    void moveTextBufferLeft();
    void moveTextBufferRight();
    std::vector<BaseTextBuffer *> &textBuffersList();
    const std::vector<BaseTextBuffer *> &textBuffersList() const;
    void setActiveTextBuffer(BaseTextBuffer *);
    const BaseTextBuffer *activeTextBuffer() const;
    BaseTextBuffer *activeTextBuffer();
    virtual int maxHeight() const;
    virtual int minHeight() const;
  Signal<void (BaseTextBuffer *)> setTextBuffer;
  Signal<void (BaseTextBuffer *)> deleteTextBuffer;
private:
    virtual void paintEvent(PaintEvent &);
    std::vector<BaseTextBuffer *> textBuffersList_;
    BaseTextBuffer *activeTextBuffer_;
    
};
