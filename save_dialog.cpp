#include "save_dialog.hpp"
#include "to_utf16.hpp"
#include "to_utf8.hpp"
#include "text_file.hpp"
#include "screen.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/param.h>

SaveDialog::SaveDialog(Screen *screen, TextFile *textFile):
    screen_(screen),
    textFile_(textFile)
{
    scanDirectory();
}

void SaveDialog::internalInsert(Coord &cursor, std::wstring value)
{
    if (cursor.y == 1 && value.find(L"\n") == std::wstring::npos)
        BaseTextBuffer::internalInsert(cursor, value);
    if (value.find(L"\n") != std::wstring::npos && cursor.y > 0)
    {
        struct stat buf;
        auto fileName = toUtf8(buffer_[screen_->cursor().y]);
        stat(fileName.c_str(), &buf);
        if (S_ISDIR(buf.st_mode))
        {
            auto folderName = buffer_[0];
            folderName = { begin(folderName) + folderName.rfind(L"/") + 1, end(folderName) - 1 };
            chdir(fileName.c_str());
            scanDirectory();
            int line = -1;
            for (size_t i = 0; i < buffer_.size(); ++i)
                if (buffer_[i] == folderName)
                {
                    line = i;
                    break;
                }
            if (line != -1)
                cursor = { 0, line };
            else
                cursor = { 0, 1 };
        }
        else
            saveAs(this, textFile_, fileName);
    }
}

std::wstring SaveDialog::internalDelete(const Coord cursor, int value)
{
    if (cursor.y == 1 && cursor.x <= static_cast<int>(buffer_[screen_->cursor().y].size()) - value)
        return BaseTextBuffer::internalDelete(cursor, value);
    else 
        return L"";
}

std::wstring SaveDialog::internalBackspace(Coord &cursor, int value)
{
    if (cursor.y == 1 && cursor.x >= value)
        return BaseTextBuffer::internalBackspace(cursor, value);
    else
        return L"";
}


void SaveDialog::scanDirectory()
{
    buffer_.clear();
    char *tmp = getcwd(nullptr, MAXPATHLEN);
    auto currentDir = toUtf16(tmp);
    free(tmp);
    setName(std::wstring{begin(currentDir) + currentDir.rfind(L'/'), end(currentDir)});
    buffer_.push_back(currentDir + L":");

    auto d = opendir(".");
    buffer_.push_back(L"");
    while (auto de = readdir(d))
    {
        auto fileName = toUtf16(de->d_name);
        if (fileName == L".." || 
            (fileName.size() > 0 && fileName[0] != L'.' && fileName[fileName.size() - 1] != L'~'))
            buffer_.push_back(fileName);
    }
    closedir(d);
    std::sort(begin(buffer_) + 2, end(buffer_));
}
