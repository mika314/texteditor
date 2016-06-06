#pragma once

template <typename Ret, typename... Args>
class Signal;

template <typename Ret, typename... Args>
class Signal<Ret (Args...)>
{
public:
  template <typename Type>
  void connect(Type *o, Ret (Type::*m)(Args...))
  {
    object = (FakeClass *)o;
    typedef Ret (FakeClass::*FakeMethod)(Args...);
    method = (FakeMethod)m;
  }
  Ret operator()(Args&&... args)
  {
    return (object->*method)(args...);
  }
  Ret operator()(const Args&... args)
  {
    return (object->*method)(args...);
  }
private:
  class FakeClass {
  public:
    Ret defaultMethod(Args...)
      {
        return Ret();
      }
  };
  FakeClass *object = nullptr;
  Ret (FakeClass::*method)(Args...) = &FakeClass::defaultMethod;
};
