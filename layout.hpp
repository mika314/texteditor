#pragma once
#include "layoutable.hpp"
#include <vector>

class Widget;

class Layout: public Layoutable
{
public:
    enum Style { Vertical, Horizontal };
    Layout(Style);
    void addWidget(Widget *);
    void addLayout(Layout *);
    virtual void setLeft(int);
    virtual void setTop(int);
    virtual void resize(int width, int height);
    virtual int maxHeight() const;
    virtual int minHeight() const;
    virtual int maxWidth() const;
    virtual int minWidth() const;
private:
    std::vector<Layoutable *> layoutablesList_;
    Style style_;
    int left_;
    int top_;
    int width_;
    int height_;
};
