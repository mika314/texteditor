#include "paint_event.hpp"
#include "resize_event.hpp"
#include "widget.hpp"
#include "application.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <stdexcept>
#include <iostream>

Widget::Widget(Widget *parent):
    parent_(parent),
    width_(640),
    height_(480),
    left_(0),
    top_(0)
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

    texture_ = SDL_CreateTexture(ancestor()->renderer_, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width(), height());
}

Widget::~Widget()
{
    SDL_DestroyTexture(texture_);
    if (!parent_)
    {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        Application::instance()->removeWidget(this);
    }
    else
        parent_->removeChild(this);
}

int Widget::width() const
{
    return width_;
}

void Widget::resizeTexture()
{
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTexture(ancestor()->renderer_, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width(), height());
}

void Widget::setWidth(int value)
{
    width_ = value;
    resizeTexture();
}

int Widget::height() const
{
    return height_;
}

void Widget::setHeight(int value)
{
    height_ = value;
    resizeTexture();
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

bool Widget::keyPressEvent(KeyEvent &)
{
    return false;
}

bool Widget::keyReleaseEvent(KeyEvent &)
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
    width_ = event.width;
    height_ = event.height;
    resizeTexture();

    PaintEvent e;
    SDL_LockTexture(texture_, nullptr, (void **)&e.pixels, &e.pitch);
    for (int y = 0; y < height(); ++y)
        for (int x = 0; x < width(); ++x)
        {
            e.pixels[y * e.pitch + 3 * x] = (windowId() * 20) % 256;
            e.pixels[y * e.pitch + 3 * x + 1] = 0;
            e.pixels[y * e.pitch + 3 * x + 2] = 0;
        }
    SDL_UnlockTexture(texture_);
}

void Widget::addChild(Widget *w)
{
    children_.push_back(w);
}

void Widget::removeChild(Widget *w)
{
    children_.erase(std::remove(begin(children_), end(children_), w), end(children_));
}


void Widget::internalPaint()
{
    PaintEvent event;
    SDL_LockTexture(texture_, nullptr, (void **)&event.pixels, &event.pitch);
    paintEvent(event);
    SDL_UnlockTexture(texture_);
    SDL_Rect gRect;
    gRect.x = 0;
    gRect.y = 0;
    auto w = this;
    while (w)
    {
        gRect.x += w->left();
        gRect.y += w->top();
        w = w->parent();
    }
    gRect.w = width();
    gRect.h = height();
    SDL_RenderCopy(ancestor()->renderer_, texture_, nullptr, &gRect);
    for (auto child: children())
        child->internalPaint();
}
