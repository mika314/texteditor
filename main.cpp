#include "widget.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "base_text_buffer.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Screen w2(&w);
    w2.setLeft(0);
    w2.setTop(0);
    w2.setWidth(640);
    w2.setHeight(480);
    BaseTextBuffer b;
    w2.setTextBuffer(&b);
    w2.setFocus();
    return a.exec();
}
