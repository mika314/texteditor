#pragma once
#include "base_text_buffer.hpp"

class Screen;

class IsearchBuffer: public BaseTextBuffer
{
public:
  IsearchBuffer(Screen *);
  void findNext();
private:
  Screen *screen_;
  std::wstring searchString_;
  Coord initialCursor_;
  bool search();
  virtual void postInsert(Coord &cursor, const std::wstring &);
  virtual int preBackspace(Coord &cursor, int = 1);
  virtual void postBackspace(Coord &cursor, int = 1);
};
