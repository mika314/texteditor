#include "widget.hpp"
#include "application.hpp"
#include "screen.hpp"
#include "text_file.hpp"

int main(int argc, char **argv)
{
    Application a(argc, argv);
    Widget w;
    
    Screen w2(&w);
    TextFile f("text.txt");
    w2.setLeft(0);
    w2.setTop(0);
    w2.setWidth(640);
    w2.setHeight(480);
    w2.setFocus();
    w2.setTextBuffer(&f);
    auto r = a.exec();
    f.save();
    return r;
}
