#pragma once
#include "base_text_buffer.hpp"
#include <functional>

class Screen;
class TextFile;

class SaveDialog: public BaseTextBuffer
{
public:
  SaveDialog(Screen *screen, TextFile *textFile);
  std::function<void (SaveDialog *, TextFile *, const std::string &)> saveAs;
private:
  Screen *screen_;
  TextFile *textFile_;
  void scanDirectory();
  virtual std::wstring preInsert(Coord &cursor, const std::wstring &);
  virtual int preDelete(const Coord cursor, int = 1);
  virtual int preBackspace(Coord &cursor, int = 1);
};
