#pragma once
#include <string>

class TextInputEvent
{
public:
  TextInputEvent(const std::wstring &);
  std::wstring text() const;
private:
  std::wstring text_;
};
