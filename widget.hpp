#pragma once
#include <vector>

class KeyEvent;
class MouseEvent;
class ResizeEvent;
class PaintEvent;
struct SDL_Window;

class Widget
{
public:
    Widget(Widget *parent = nullptr);
    virtual ~Widget();
    int width() const;
    int height() const;
    Widget *parent() const;
    const std::vector<Widget *> &children() const;
protected:
    // some of following events handlers return bool, true means event handled and does not require handling from the parent object
    virtual bool keyPressEvent(KeyEvent &);
    virtual bool keyReleaseEvent(KeyEvent &);
    virtual bool mouseDoubleClickEvent(MouseEvent &);
    virtual bool mouseMoveEvent(MouseEvent &);
    virtual bool mousePressEvent(MouseEvent &);
    virtual bool mouseReleaseEvent(MouseEvent &);
    virtual void paintEvent(PaintEvent &);
    virtual void resizeEvent(ResizeEvent &);
private:
    SDL_Window *window_;
    Widget *parent_;
    void addChild(Widget *);
    void removeChild(Widget *);
};
