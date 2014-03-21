#include "to_utf8.hpp"
#include <cstring>
#include <cwchar>

std::string toUtf8(const std::wstring &value)
{
    return std::string{ begin(value), end(value) };
}

std::string toUtf8(const wchar_t *value)
{
    return std::string{ value, value + wcslen(value) };
}
