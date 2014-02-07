#pragma once
#include <SDL2/SDL.h>
#include <vector>

class KeyEvent;
class MouseEvent;
class ResizeEvent;
class PaintEvent;
struct SDL_Window;
struct SDL_Renderer;

class Widget
{
    friend class Application;
public:
    Widget(Widget *parent = nullptr);
    virtual ~Widget();
    int width() const;
    int height() const;
    Widget *parent() const;
    const std::vector<Widget *> &children() const;
    Uint32 windowId() const;
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
    std::vector<Widget *> children_;
    SDL_Renderer *renderer_;
    void addChild(Widget *);
    void removeChild(Widget *);
};
