#include "to_utf8.hpp"

std::string toUtf8(const std::wstring &value)
{
    return { begin(value), end(value) };
}
