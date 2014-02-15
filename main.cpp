#include "widget.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "file_buffer.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Screen w2(&w);
    FileBuffer f("key_event.hpp");
    w2.setLeft(0);
    w2.setTop(0);
    w2.setWidth(640);
    w2.setHeight(480);
    w2.setFocus();
    return a.exec();
}
