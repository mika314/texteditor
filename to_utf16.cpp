#include "to_utf16.hpp"
#include <cstring>

std::wstring toUtf16(const char *value)
{
    // TODO
    return std::wstring(value, value + strlen(value));
}

std::wstring toUtf16(const std::string &value)
{
    // TODO
    return std::wstring{ begin(value), end(value) };
}
