#pragma once
#include "widget.hpp"
#include <vector>
#include <functional>

class BaseTextBuffer;
class Screen;

class Tabs: public Widget
{
public:
  Tabs(Widget *parent);
  void addTextBuffer(BaseTextBuffer *);
  void closeTextBuffer(BaseTextBuffer *);
  void closeActiveTextBuffer();
  void switchToNextTextBuffer();
  void switchToPrevTextBuffer();
  void moveTextBufferLeft();
  void moveTextBufferRight();
  std::vector<BaseTextBuffer *> &textBuffersList();
  const std::vector<BaseTextBuffer *> &textBuffersList() const;
  void setActiveTextBuffer(BaseTextBuffer *);
  const BaseTextBuffer *activeTextBuffer() const;
  BaseTextBuffer *activeTextBuffer();
  virtual int maxHeight() const;
  virtual int minHeight() const;
  std::function<void (BaseTextBuffer *)> setTextBuffer;
  std::function<void (BaseTextBuffer *)> deleteTextBuffer;
private:
  virtual void paintEvent(PaintEvent &);
  std::vector<BaseTextBuffer *> textBuffersList_;
  BaseTextBuffer *activeTextBuffer_;
    
};
