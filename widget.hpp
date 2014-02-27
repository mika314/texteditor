#pragma once
#include "paint_device.hpp"
#include "layoutable.hpp"
#include <SDL2/SDL.h>
#include <vector>

class KeyEvent;
class TextInputEvent;
class MouseEvent;
struct ResizeEvent;
class PaintEvent;
class Layout;
struct SDL_Window;
struct SDL_Renderer;

class Widget: public PaintDevice, public Layoutable
{
    friend class Application;
public:
    Widget(Widget *parent = nullptr);
    virtual ~Widget();
    virtual void resize(int width, int height);
    int width() const;
    void setWidth(int);
    int height() const;
    void setHeight(int);
    int left() const;
    virtual void setLeft(int);
    int top() const;
    virtual void setTop(int);
    Widget *parent() const;
    Widget *ancestor();
    void setFocus();
    void clearFocus();
    bool hasFocus();
    const std::vector<Widget *> &children() const;
    Uint32 windowId() const;
    virtual SDL_Renderer *renderer();
    virtual int gLeft() const;
    virtual int gTop() const;
    void update();
    void setLayout(Layout *);
    Layout *layout();
    const Layout *layout() const;
protected:
    // some of following events handlers return bool, true means event handled and does not require handling from the parent object
    virtual bool keyPressEvent(KeyEvent &);
    virtual bool keyReleaseEvent(KeyEvent &);
    virtual bool textInputEvent(TextInputEvent &);
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
    int width_;
    int height_;
    int left_;
    int top_;
    bool needRepaint_;
    Layout *layout_;
    void addChild(Widget *);
    void removeChild(Widget *);
    void internalPaint(PaintEvent &);
    Widget(const Widget &);
    Widget &operator=(const Widget &);
    bool needRepaint() const;
};
