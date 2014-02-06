#pragma once

class Widget;

class Application
{
    friend class Widget;
public:
    Application(int &argc, char **argv);
    ~Application();
    int exec();
    static Application *instance();
private:
    static Application *instance_;
    void addWidget(Widget *);
    void removeWidget(Widget *);
};
