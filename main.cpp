#include "widget.hpp"
#include "application.hpp"
#include "screen.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Screen w2(&w);
    w2.setLeft(100);
    w2.setTop(100);
    w2.setWidth(500);
    w2.setHeight(100);
    return a.exec();
}
