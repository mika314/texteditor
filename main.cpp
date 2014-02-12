#include "widget.hpp"
#include "application.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Widget w2(&w);
    w2.setLeft(100);
    w2.setTop(100);
    w2.setWidth(100);
    w2.setHeight(100);
    return a.exec();
}
