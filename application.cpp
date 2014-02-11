#include "application.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>
#include <string>

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
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
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

void Application::addWidget(Widget *)
{
    
}

void Application::removeWidget(Widget *)
{
    
}
