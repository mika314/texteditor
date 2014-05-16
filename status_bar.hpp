#pragma once
#include "text_screen.hpp"

class StatusBar: public TextScreen
{
public:
    StatusBar(Widget *parent);
    virtual int maxHeight() const;
    virtual int minHeight() const;
};

