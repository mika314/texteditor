#pragma once
#include "screen.hpp"

class StatusBar: public Screen
{
public:
  StatusBar(Widget *parent);
  virtual int maxHeight() const;
  virtual int minHeight() const;
};

