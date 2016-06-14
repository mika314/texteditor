#pragma once

class Layout;

class Layoutable
{
public:
  Layoutable();
  virtual ~Layoutable();
  virtual void setLeft(int) = 0;
  virtual void setTop(int) = 0;
  virtual void resize(int width, int height) = 0;
  virtual int maxHeight() const = 0;
  virtual int minHeight() const = 0;
  virtual int maxWidth() const = 0;
  virtual int minWidth() const = 0;
  void setParentLayout(Layout *);
  Layout *parentLayout() const;
private:
  Layout *parentLayout_;
};
