#pragma once

class BaseDeletingObject
{
public:
    virtual ~BaseDeletingObject()
    {}
};

template <typename T>
class DeletingObject: public BaseDeletingObject
{
public:
    DeletingObject(T *obj):
        obj_(obj)
    {}
    virtual ~DeletingObject()
    {
        delete obj_;
    }
private:
    T *obj_;
};
