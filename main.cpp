#include "widget.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "base_text_buffer.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Screen w2(&w);
    w2.setLeft(100);
    w2.setTop(100);
    w2.setWidth(500);
    w2.setHeight(100);
    BaseTextBuffer b;
    w2.setTextBuffer(&b);
    w2.setFocus();
    return a.exec();
}
