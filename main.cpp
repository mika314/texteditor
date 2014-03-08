#include "widget.hpp"
#include "status_bar.hpp"
#include "layout.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "open_dialog.hpp"
#include "isearch_buffer.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget mainWindow;
    Screen screen(&mainWindow);
    OpenDialog openDialog;
    screen.setFocus();
    screen.setTextBuffer(&openDialog);
    StatusBar statusBar(&mainWindow);
    screen.setStatusBar(&statusBar);
    Layout mainWindowLayout(Layout::Vertical);
    mainWindow.setLayout(&mainWindowLayout);
    mainWindowLayout.addWidget(&screen);
    mainWindowLayout.addWidget(&statusBar);
    auto r = a.exec();
    return r;
}
