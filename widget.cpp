#include "widget.hpp"
#include "application.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <stdexcept>

Widget::Widget(Widget *parent):
    parent_(parent)
{
    if (!parent_)
    {
        window_ = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
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
        SDL_DestroyWindow(window_);
        SDL_DestroyRenderer(renderer_);
        Application::instance()->removeWidget(this);
    }
    else
        parent_->removeChild(this);
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
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}

void Widget::resizeEvent(ResizeEvent &)
{
}

void Widget::addChild(Widget *w)
{
    children_.push_back(w);
}

void Widget::removeChild(Widget *w)
{
    children_.erase(std::remove(begin(children_), end(children_), w), end(children_));
}
