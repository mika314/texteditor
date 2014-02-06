#include "widget.hpp"
#include <SDL2/SDL.h>

Widget::Widget(Widget *parent):
    parent_(parent)
{
    if (!parent_)
        window_ = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    else
        parent_->addChild(this);
}

Widget::~Widget()
{
    if (!parent_)
        SDL_DestroyWindow(window_);
    else
        parent_->removeChild(this);
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

void Widget::resizeEvent(ResizeEvent &)
{
}

void Widget::addChild(Widget *)
{
}

void Widget::removeChild(Widget *)
{
}
