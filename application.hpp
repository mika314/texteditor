#pragma once
#include "deliting_object.hpp"
#include <vector>
#include <X11/Xlib.h>

class Widget;

class Application
{
    friend class Widget;
public:
    Application(int &argc, char **argv);
    ~Application();
    template <typename T>
    void queueDelete(T *obj)
    {
        deletingObjects_.push_back(new DeletingObject<T>(obj));
    }
    int exec();
    Display *display() const;
    int screen() const;
    static Application *instance();
private:
    static Application *instance_;
    std::vector<Widget *> widgetList_;
    Widget *focusWidget_;
    Widget *needUpdateWithoutRedraw_;
    std::vector<BaseDeletingObject *> deletingObjects_;
    Display *display_;
    int screen_;
    void addWidget(Widget *);
    void removeWidget(Widget *);
    Widget *widgetByWindowId(Window);
    void setFocusWidget(Widget *);
    Widget *focusWidget() const;
    void clearFocus();
};
