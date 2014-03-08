#include "open_dialog.hpp"
#include "to_utf16.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

OpenDialog::OpenDialog()
{
    auto d = opendir(".");
    while (auto de = readdir(d))
        buffer_.push_back(toUtf16(de->d_name));
    closedir(d);
    setReadOnly(true);
}

void OpenDialog::insert(Coord &cursor, std::wstring value)
{
    if (value.find(L"\n") != std::wstring::npos)
    {
        // TODO
    }
    BaseTextBuffer::insert(cursor, value);
}

