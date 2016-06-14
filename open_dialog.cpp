#include "open_dialog.hpp"
#include "to_utf16.hpp"
#include "to_utf8.hpp"
#include "text_file.hpp"
#include "screen.hpp"
#include "current_dir.hpp"
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
  scanDirectory();
  setReadOnly(true);
}

void OpenDialog::postInsert(Coord &cursor, const std::wstring &value)
{
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
    {
      openFile(this, fileName);
      cursor = { 0, 0 };
    }
  }
}

void OpenDialog::scanDirectory()
{
  buffer_.clear();
  auto currentDir = toUtf16(getCurrentDir());
  setName(std::wstring{begin(currentDir) + currentDir.rfind(L'/') + 1, end(currentDir)});
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
}
