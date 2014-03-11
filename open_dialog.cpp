#include "open_dialog.hpp"
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

OpenDialog::OpenDialog(Screen *screen):
    screen_(screen)
{
    char *tmp = get_current_dir_name();
    auto currentDir = toUtf16(tmp);
    free(tmp);
    buffer_.push_back(currentDir + L":");

    auto d = opendir(".");
    while (auto de = readdir(d))
    {
        auto fileName = toUtf16(de->d_name);
        if (fileName == L".." || 
            (fileName.size() > 0 && fileName[0] != L'.' && fileName[fileName.size() - 1] != L'~'))
            buffer_.push_back(fileName);
    }
    closedir(d);
    std::sort(begin(buffer_) + 1, end(buffer_));
    setReadOnly(true);
}

void OpenDialog::insert(Coord &cursor, std::wstring value)
{
    BaseTextBuffer::insert(cursor, value);
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
            auto newOpenDialog = std::make_shared<OpenDialog>(screen_);
            int line = -1;
            for (size_t i = 0; i < newOpenDialog->buffer_.size(); ++i)
                if (newOpenDialog->buffer_[i] == folderName)
                {
                    line = i;
                    break;
                }
            screen_->setTextBuffer(newOpenDialog);
            if (line != -1)
                screen_->setCursor({ 0, line });
        }
        else
            screen_->setTextBuffer(std::make_shared<TextFile>(fileName));
    }
}

