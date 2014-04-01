#include "dialog.hpp"
#include <sstream>

Dialog::Dialog(std::wstring message, Answers answers):
    message_(message),
    answers_(answers)
{
    std::wostringstream tmp;
    tmp << message;
    bool first = true;
    if ((answers_ & Yes) != 0)
    {
        if (first)
            tmp << L" (";
        else
            tmp << L"/";
        tmp << L"y";
        first = false;
    }
    if ((answers_ & No) != 0)
    {
        if (first)
            tmp << L"(";
        else
            tmp << L"/";
        tmp << L"n";
        first = false;
    }
    if ((answers_ & Cancel) != 0)
    {
        if (first)
            tmp << L"(";
        else
            tmp << L"/";
        tmp << L"c";
        first = false;
    }
    if (!first)
        tmp << L"): ";
    buffer_.push_back(tmp.str());
}

std::wstring Dialog::preInsert(Coord &cursor, std::wstring value)
{
    if ((answers_ & Yes) != 0 && value.find(L'y') != std::wstring::npos)
        result(Yes);
    if ((answers_ & No) != 0 && value.find(L'n') != std::wstring::npos)
        result(No);
    if ((answers_ & Cancel) != 0 && value.find(L'c') != std::wstring::npos)
        result(Cancel);
    return L"";
}

int Dialog::preBackspace(Coord &cursor, int value)
{
    return 0;
}
