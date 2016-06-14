#include "to_utf16.hpp"
#include <cstring>

std::wstring toUtf16(const char *value)
{
  return toUtf16(std::string{value});
}

std::wstring toUtf16(const std::string &value)
{
  std::wstring result;
  wchar_t tmp = 0;
  for (auto ch: value)
  {
    if ((ch & 0x80) == 0)
    {
      if (tmp != 0)
        result += tmp;
      tmp = 0;
      result += ch;
    }
    else if ((ch & 0xe0) == 0xc0)
    {
      if (tmp != 0)
        result += tmp;
      tmp = ch & 0x1f;
    }
    else if ((ch & 0xf0) == 0xe0)
      tmp = ch & 0xf;
    else if ((ch & 0xc0) == 0x80)
      tmp = (tmp << 6) | (ch & 0x3f);
  }
  if (tmp != 0)
    result += tmp;
  return result;
}
