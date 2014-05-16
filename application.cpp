#include "to_utf16.hpp"
#include "paint_event.hpp"
#include "resize_event.hpp"
#include "key_event.hpp"
#include "text_input_event.hpp"
#include "application.hpp"
#include "widget.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>

Application *Application::instance_ = nullptr;

Application::Application(int &argc, char **argv):
    focusWidget_(nullptr),
    needUpdateWithoutRedraw_(nullptr)
{
    if (instance_ != nullptr)
        throw std::runtime_error("The program can have only one instance of Application");
    instance_ = this;

	display_ = XOpenDisplay((char *)0);
    screen_ = DefaultScreen(display_);
}

Application::~Application()
{
    SDL_Quit();
    instance_ = nullptr;
}

int Application::exec()
{
    bool done = false;
    while (!done)
    {
        XEvent event;
		XNextEvent(display_, &event);

        if (event.type==Expose && event.xexpose.count==0) 
        {
            for (auto w: widgetList_)
                if (w->needRepaint())
                {
                    PaintEvent e;
                    w->internalPaint(e);
                }
		}


        /*
        if (SDL_WaitEvent(&e))
        {
            switch (e.type)
            {
            case SDL_WINDOWEVENT:
                {
                    Widget *w = widgetByWindowId(e.window.windowID);
                    switch (e.window.event)
                    {
                    case SDL_WINDOWEVENT_SHOWN:
                        std::cout << "Window " << e.window.windowID << " shown" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        std::cout << "Window " << e.window.windowID << " hidden" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        {
                            needUpdateWithoutRedraw_ = w;
                            break;
                        }
                    case SDL_WINDOWEVENT_MOVED:
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        {
                            w->resize(e.window.data1, e.window.data2);
#if __APPLE__==1
                            SDL_RenderPresent(w->renderer_); // hack for MacOS X
#endif
                            break;
                        }
                    case SDL_WINDOWEVENT_MINIMIZED:
                        std::cout << "Window " << e.window.windowID << " minimized" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        std::cout << "Window " << e.window.windowID << " maximized" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        std::cout << "Window " << e.window.windowID << " restored" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        std::cout << "Mouse entered window " << e.window.windowID << std::endl;
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        std::cout << "Mouse left window " << e.window.windowID << std::endl;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        std::cout << "Window " << e.window.windowID << " gained keyboard focus" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        std::cout << "Window " << e.window.windowID << " lost keyboard focus" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        std::cout << "Window " << e.window.windowID << " closed" << std::endl;
                        break;
                    default:
                        std::cout << "Window " << e.window.windowID << " got unknown event " << static_cast<int>(e.window.event) << std::endl;
                        break;
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), static_cast<bool>(e.key.repeat) };
                    auto w = focusWidget();
                    if (!w)
                        w = widgetByWindowId(e.key.windowID);
                    else if (w->ancestor() != widgetByWindowId(e.key.windowID))
                    {
                        std::cerr << "Unknown windowID " << e.key.windowID << std::endl;
                        break;
                    }
                    while (w)
                    {
                        if (w->keyPressEvent(ke))
                            break;
                        w = w->parent();
                    }
                    break;
                }
            case SDL_KEYUP:
                {
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), static_cast<bool>(e.key.repeat) };
                    auto w = focusWidget();
                    if (!w)
                        w = widgetByWindowId(e.key.windowID);
                    else if (w->ancestor() != widgetByWindowId(e.key.windowID))
                    {
                        std::cerr << "Unknown windowID " << e.key.windowID << std::endl;
                        break;
                    }

                    while (w)
                    {
                        if (w->keyReleaseEvent(ke))
                            break;
                        w = w->parent();
                    }
                    break;
                }
            case SDL_TEXTINPUT:
                {
                    TextInputEvent tie { toUtf16(e.text.text) };
                    auto w = focusWidget();
                    if (!w)
                        w = widgetByWindowId(e.key.windowID);
                    else if (w->ancestor() != widgetByWindowId(e.key.windowID))
                    {
                        std::cerr << "Unknown windowID " << e.key.windowID << std::endl;
                        break;
                    }

                    while (w)
                    {
                        if (w->textInputEvent(tie))
                            break;
                        w = w->parent();
                    }
                    break;
                }
            case SDL_QUIT:
                done = true;
                break;
            }
        }
        const auto isEmpty = (SDL_PeepEvents(&e, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) == 0);
        if (isEmpty || SDL_GetTicks() > lastUpdate_ + 50)
        {
            for (auto w: widgetList_)
                if (w->needRepaint())
                {
                    PaintEvent e;
                    w->internalPaint(e);
                }
            if (needUpdateWithoutRedraw_)
            {
                needUpdateWithoutRedraw_->updateWithoutRedraw();
                needUpdateWithoutRedraw_ = nullptr;
            }
            lastUpdate_ = SDL_GetTicks();
        }
        */
        for (auto obj: deletingObjects_)
            delete obj;
        deletingObjects_.clear();
    }
    return 0;
}

Display *Application::display() const
{
    return display_;
}
int Application::screen() const
{
    return screen_;
}


Application *Application::instance()
{
    return instance_;
}

void Application::addWidget(Widget *w)
{
    widgetList_.push_back(w);
}

void Application::removeWidget(Widget *w)
{
    widgetList_.erase(std::remove(begin(widgetList_), end(widgetList_), w), end(widgetList_));
}

void Application::setFocusWidget(Widget *value)
{
    focusWidget_ = value;
}

Widget *Application::focusWidget() const
{
    return focusWidget_;
}

void Application::clearFocus()
{
    focusWidget_ = nullptr;
}
