#pragma once

class Layoutable
{
public:
    ~Layoutable();
    virtual void setLeft(int) = 0;
    virtual void setTop(int) = 0;
    virtual void resize(int width, int height) = 0;
};
