#include "widget.hpp"
#include "painter.hpp"
#include "paint_event.hpp"
#include "resize_event.hpp"
#include "layout.hpp"
#include "application.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>

Widget::Widget(Widget *parent):
    parent_(parent),
    width_(640),
    height_(480),
    left_(0),
    top_(0),
    needRepaint_(true),
    layout_(nullptr)
{
    if (!parent_)
    {
        window_ = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width(), height(), SDL_WINDOW_RESIZABLE);
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer_ == nullptr)
            throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
        Application::instance()->addWidget(this);
    }
    else
        parent_->addChild(this);
}

Widget::~Widget()
{
    if (!parent_)
    {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        Application::instance()->removeWidget(this);
    }
    else
        parent_->removeChild(this);
}

void Widget::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    ResizeEvent e;
    e.width = width;
    e.height = height;
    if (layout_)
    {
        layout_->resize(width, height);
        layout_->setLeft(0);
        layout_->setTop(0);
    }
    resizeEvent(e);
    update();
}

int Widget::width() const
{
    return width_;
}

void Widget::setWidth(int value)
{
    resize(value, height());
}

int Widget::height() const
{
    return height_;
}

void Widget::setHeight(int value)
{
    resize(width(), value);
}

int Widget::left() const
{
    return left_;
}

void Widget::setLeft(int value)
{
    left_ = value;
}

int Widget::top() const
{
    return top_;
}

void Widget::setTop(int value)
{
    top_ = value;
}

Widget *Widget::parent() const
{
    return parent_;
}

Widget *Widget::ancestor() 
{
    Widget *res = this;
    while (res->parent())
        res = res->parent();
    return res;
}


void Widget::setFocus()
{
    Application::instance()->setFocusWidget(this);
}

void Widget::clearFocus()
{
    if (hasFocus())
        Application::instance()->clearFocus();
}

bool Widget::hasFocus()
{
    return Application::instance()->focusWidget() == this;
}

const std::vector<Widget *> &Widget::children() const
{
    return children_;
}

Uint32 Widget::windowId() const
{
    return SDL_GetWindowID(window_);
}

SDL_Renderer *Widget::renderer()
{
    return ancestor()->renderer_;
}

int Widget::gLeft() const
{
    int res = 0;
    auto w = this;
    while (w)
    {
        res += w->left();
        w = w->parent();
    }
    return res;
}

int Widget::gTop() const
{
    int res = 0;
    auto w = this;
    while (w)
    {
        res += w->top();
        w = w->parent();
    }
    return res;
}

void Widget::update()
{
    ancestor()->needRepaint_ = true;
}

void Widget::setLayout(Layout *value)
{
    layout_ = value;
    ancestor()->resize(width_, height_);
}

Layout *Widget::layout()
{
    return layout_;
}

const Layout *Widget::layout() const
{
    return layout_;
}

int Widget::maxHeight() const
{
    return std::numeric_limits<int>::max();
}

int Widget::minHeight() const
{
    return 0;
}

int Widget::maxWidth() const
{
    return std::numeric_limits<int>::max();
}

int Widget::minWidth() const
{
    return 0;
}

bool Widget::keyPressEvent(KeyEvent &)
{
    return false;
}

bool Widget::keyReleaseEvent(KeyEvent &)
{
    return false;
}

bool Widget::textInputEvent(TextInputEvent &)
{
    return false;
}

bool Widget::mouseDoubleClickEvent(MouseEvent &)
{
    return false;
}

bool Widget::mouseMoveEvent(MouseEvent &)
{
    return false;
}

bool Widget::mousePressEvent(MouseEvent &)
{
    return false;
}

bool Widget::mouseReleaseEvent(MouseEvent &)
{
    return false;
}

void Widget::paintEvent(PaintEvent &)
{
}

void Widget::resizeEvent(ResizeEvent &event)
{
    std::cout << " resizeEvent: " << event.width << "x" << event.height << std::endl;
}

void Widget::addChild(Widget *w)
{
    children_.push_back(w);
}

void Widget::removeChild(Widget *w)
{
    children_.erase(std::remove(begin(children_), end(children_), w), end(children_));
}


void Widget::internalPaint(PaintEvent &event)
{
    paintEvent(event);
    for (auto child: children())
        child->internalPaint(event);
    needRepaint_ = false;
}

bool Widget::needRepaint() const
{
    return needRepaint_;
}
