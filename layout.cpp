#include "layout.hpp"
#include "widget.hpp"

Layout::Layout(Style style):
    style_(style),
    left_(0),
    top_(0),
    width_(0),
    height_(0)
{
}

void Layout::addWidget(Widget *widget)
{
    layoutablesList_.push_back(widget);
}

void Layout::addLayout(Layout *layout)
{
    layoutablesList_.push_back(layout);
}

void Layout::setLeft(int value)
{
    left_ = value;
}

void Layout::setTop(int value)
{
    top_ = value;
}

void Layout::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    if (style_ == Vertical)
    {
        int i = 0;
        for (auto &l: layoutablesList_)
        {
            l->setLeft(left_);
            l->setTop(top_ + i * height / layoutablesList_.size());
            l->resize(width, (i + 1) * height / layoutablesList_.size() - 
                      i * height / layoutablesList_.size());
            ++i;
        }
    }
    else // if style == Horizontal
    {
        int i = 0;
        for (auto &l: layoutablesList_)
        {
            l->setLeft(left_ + i * width / layoutablesList_.size());
            l->setTop(top_);
            l->resize((i + 1)* width / layoutablesList_.size() - 
                      i * width / layoutablesList_.size(), height);
            ++i;
        }
    }
}

