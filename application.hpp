#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Widget;

class Application
{
    friend class Widget;
public:
    Application(int &argc, char *argv[]);
    ~Application();
    int exec();
    static Application *instance();
private:
    static Application *instance_;
    std::vector<Widget *> widgetList_;
    Widget *focusWidget_;
    void addWidget(Widget *);
    void removeWidget(Widget *);
    Widget *widgetByWindowId(Uint32);
    void setFocusWidget(Widget *);
    Widget *focusWidget() const;
    void clearFocus();
};
