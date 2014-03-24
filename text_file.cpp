#include "text_file.hpp"
#include "to_utf16.hpp"
#include "to_utf8.hpp"
#include <fstream>

TextFile::TextFile(std::string fileName):
    fileName_(fileName)
{
    if (!fileName.empty())
        setName(toUtf16(fileName));
    else
        setName(L"Untitled");
        
    std::ifstream f(fileName_);
    if (f.is_open())
        while (!f.eof())
        {
            std::string line;
            std::getline(f, line);
            buffer_.push_back(toUtf16(line));
        }
    else
        buffer_.push_back(L"");
}

std::string TextFile::fileName() const
{
    return fileName_;
}

void TextFile::save()
{
    if (!fileName_.empty())
        saveAs(fileName_);
}

void TextFile::saveAs(std::string fileName)
{
    fileName_ = fileName;
    setName(toUtf16(fileName));
    std::ofstream f(fileName_);
    for (const auto &l: buffer_)
        f << toUtf8(l) << std::endl;
    clearModified();
}
