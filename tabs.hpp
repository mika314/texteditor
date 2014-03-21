#pragma once
#include "widget.hpp"
#include <vector>

class BaseTextBuffer;
class Screen;

class Tabs: public Widget
{
public:
    Tabs(Widget *parent, Screen *);
    void addTextBuffer(BaseTextBuffer *);
    std::vector<BaseTextBuffer *> &textBuffersList();
    const std::vector<BaseTextBuffer *> &textBuffersList() const;
    void setActiveBuffer(BaseTextBuffer *);
    const BaseTextBuffer *activeBuffer() const;
    BaseTextBuffer *activeBuffer();
    virtual int maxHeight() const;
    virtual int minHeight() const;
private:
    virtual void paintEvent(PaintEvent &);
    std::vector<BaseTextBuffer *> textBuffersList_;
    Screen *screen_;
    BaseTextBuffer *activeBuffer_;
    
};
