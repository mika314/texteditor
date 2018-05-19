#pragma once
#include "base_text_buffer.hpp"
#include <functional>

class Screen;

class OpenDialog: public BaseTextBuffer
{
public:
  OpenDialog(Screen *screen);
  std::function<void (OpenDialog *, const std::string &)> openFile;
private:
  Screen *screen_;
  void scanDirectory();
  virtual void postInsert(Coord &cursor, const std::wstring &);
};
