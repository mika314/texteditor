#pragma once
#include "deliting_object.hpp"
#include <SDL2/SDL.h>
#include <vector>

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
    static Application *instance();
private:
    static Application *instance_;
    std::vector<Widget *> widgetList_;
    Widget *focusWidget_;
    Widget *needUpdateWithoutRedraw_;
    std::vector<BaseDeletingObject *> deletingObjects_;
    Uint32 lastUpdate_;
    void addWidget(Widget *);
    void removeWidget(Widget *);
    Widget *widgetByWindowId(Uint32);
    void setFocusWidget(Widget *);
    Widget *focusWidget() const;
    void clearFocus();
};
