#include "to_utf8.hpp"
#include <cstring>
#include <cwchar>

std::string toUtf8(const std::wstring &value)
{
    std::string result;
    for (auto ch: value)
    {
        if (ch < 0x80)
            result += static_cast<char>(ch);
        else if (ch < 0x800)
        {
            result += static_cast<char>(0xc0 | (ch >> 6));
            result += static_cast<char>(0x80 | (ch & 0x003f));
        }
        else
        {
            result += static_cast<char>(0xe0 | (ch >> 12));
            result += static_cast<char>(0x80 | ((ch >> 6) & 0x003f));
            result += static_cast<char>(0x80 | (ch & 0x003f));
        }
            
    }
    return result;
}

std::string toUtf8(const wchar_t *value)
{
    return toUtf8(std::wstring(value));
}
