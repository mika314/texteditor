#include "widget.hpp"
#include "status_bar.hpp"
#include "layout.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "text_file.hpp"
#include "isearch_buffer.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget mainWindow;
    Screen screen(&mainWindow);
    TextFile textFile("text.txt");
    screen.setFocus();
    screen.setTextBuffer(&textFile);
    StatusBar statusBar(&mainWindow);
    screen.setStatusBar(&statusBar);
    Layout mainWindowLayout(Layout::Vertical);
    mainWindowLayout.addWidget(&screen);
    mainWindowLayout.addWidget(&statusBar);
    mainWindow.setLayout(&mainWindowLayout);
    auto r = a.exec();
    textFile.save();
    return r;
}
