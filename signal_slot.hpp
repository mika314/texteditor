#pragma once

template <typename Result, typename... Arguments>
class BaseSlot
{
public:
    virtual ~BaseSlot() {}
    virtual Result operator()(Arguments...) = 0;
};

template <typename T, typename Result, typename... Arguments>
class Slot: public BaseSlot<Result, Arguments...>
{
public:
    typedef Result (T::*Method)(Arguments...);
    Slot(T *object, Method method):
        object_(object),
        method_(method)
    {}
    virtual Result operator()(Arguments... arguments)
    {
        return (object_->*method_)(arguments...);
    }
private:
    T *object_;
    Method method_;
};

template <typename Result, typename... Arguments>
class Signal
{
public:
    Signal():
        slot_(nullptr)
    {}
    ~Signal()
    {
        delete slot_;
    }
    Result operator()(Arguments... arguments)
    {
        if (slot_)
            return slot_->operator()(arguments...);
        return Result();
    }
    template <typename T>
    void connect(T *object, Result (T::*method)(Arguments...))
    {
        delete slot_;
        slot_ = new Slot<T, Result, Arguments...>(object, method);
    };
    void disconnect()
    {
        delete slot_;
        slot_ = nullptr;
    }
private:
    BaseSlot<Result, Arguments...> *slot_;
};

template <typename T, typename Result, typename... Arguments>
void connect(Signal<Result, Arguments...> &signal, T *object, Result (T::*method)(Arguments...))
{
    signal.connect(object, method);
}

template <typename Result, typename... Arguments>
void disconnect(Signal<Result, Arguments...> &signal)
{
    signal.disconnect();
}

template <typename T>
struct RemovePointer;

template <typename T>
struct RemovePointer<T *>
{
    typedef T type;
};

#define SIGNAL(o, s) (o)->s
#define SLOT(o, s) (o), &RemovePointer<decltype(o)>::type::s
