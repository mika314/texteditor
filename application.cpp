#include "paint_event.hpp"
#include "resize_event.hpp"
#include "key_event.hpp"
#include "application.hpp"
#include "widget.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cassert>

Application *Application::instance_ = nullptr;

Application::Application(int &argc, char **argv):
    focusWidget_(nullptr)
{
    if (instance_ != nullptr)
        throw std::runtime_error("The program can have only one instance of Application");
    instance_ = this;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        throw std::runtime_error("SDL_Init Error: " + std::string(SDL_GetError()));
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
        SDL_Event e;
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
                            w->update();
                            break;
                        }
                    case SDL_WINDOWEVENT_MOVED:
                        std::cout << "Window " << e.window.windowID << " moved to " << e.window.data1 << "," << e.window.data2 << std::endl;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        {
                            ResizeEvent event = { e.window.data1, e.window.data2 };
                            w->resizeEvent(event);
                            w->update();
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
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), e.key.repeat };
                    auto w = focusWidget();
                    if (!w)
                        w = widgetByWindowId(e.key.windowID);
                    else
                        assert(w->ancestor() == widgetByWindowId(e.key.windowID));
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
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), e.key.repeat };
                    auto w = focusWidget();
                    if (!w)
                        w = widgetByWindowId(e.key.windowID);
                    else
                        assert(w->ancestor() == widgetByWindowId(e.key.windowID));
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
                    std::clog << "Key: " << e.text.text << std::endl;
                    break;
                }
            case SDL_QUIT:
                done = true;
                break;
            }
            for (auto w: widgetList_)
            {
                SDL_Event e;
                if (w->needRepaint() && SDL_PeepEvents(&e, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) == 0)
                {
                    PaintEvent e;
                    w->internalPaint(e);
                    SDL_RenderPresent(w->renderer_);
                }
            }
        }
    }
    return 0;
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

Widget *Application::widgetByWindowId(Uint32 id)
{
    for (const auto w: widgetList_)
        if (id == w->windowId())
            return w;
    return nullptr;
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
