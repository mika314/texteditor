#pragma once
#include "base_text_buffer.hpp"
#include <string>
#include <functional>

class Dialog: public BaseTextBuffer
{
public:
  enum Answer { Yes = 1, No = 2, Cancel = 4};
  typedef unsigned Answers;
  Dialog(const std::wstring &message, Answers = Yes | No | Cancel);
  std::function<void (Answer)> result;
private:
  std::wstring message_;
  Answers answers_;
  virtual std::wstring preInsert(Coord &cursor, const std::wstring &value);
  virtual int preBackspace(Coord &cursor, int value);
};
