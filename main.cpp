#include "widget.hpp"
#include "status_bar.hpp"
#include "layout.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "isearch_buffer.hpp"
#include "text_file.hpp"
#include "SDL2/SDL.h"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    Widget mainWindow;
    Screen screen(&mainWindow);
    screen.setFocus();
    StatusBar statusBar(&mainWindow);
    screen.setStatusBar(&statusBar);
    Layout mainWindowLayout(Layout::Vertical);
    mainWindow.setLayout(&mainWindowLayout);
    mainWindowLayout.addWidget(&screen);
    screen.setTextBuffer(std::make_shared<TextFile>());
    mainWindowLayout.addWidget(&statusBar);
    auto r = a.exec();
    return r;
}
