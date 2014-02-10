#include "key_event.hpp"
#include "application.hpp"
#include "widget.hpp"
#include "paint_event.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>

Application *Application::instance_ = nullptr;

Application::Application(int &argc, char **argv)
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
                        {
                            PaintEvent event;
                            w->paintEvent(event);
                        }
                        break;
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    Widget *w = widgetByWindowId(e.key.windowID);
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), e.key.repeat };
                    w->keyPressEvent(ke);
                    break;
                }
            case SDL_KEYUP:
                {
                    Widget *w = widgetByWindowId(e.key.windowID);
                    KeyEvent ke { static_cast<KeyEvent::Key>(e.key.keysym.sym), SDL_GetModState(), e.key.repeat };
                    w->keyPressEvent(ke);
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
