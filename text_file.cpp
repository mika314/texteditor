#include "text_file.hpp"
#include "to_utf16.hpp"
#include "to_utf8.hpp"
#include "cpp_highlighter.hpp"
#include "full_file_name.hpp"
#include <fstream>
#include <algorithm>

static std::string baseName(std::string fileName)
{
    auto p = fileName.rfind('/');
    if (p != std::string::npos)
        return std::string{ begin(fileName) + p + 1, end(fileName) };
    else
        return fileName;
}

static bool isCpp(std::string fileName)
{
    auto p = fileName.rfind(".");
    std::string ext;
    if (p != std::string::npos)
        ext = fileName.substr(p);
    static std::string exts[] = {
        ".c",
        ".cpp",
        ".C",
        ".cc",
        ".c++",
        ".h",
        ".H",
        ".hpp",
        ".h++"
    };
    return std::find(std::begin(exts), std::end(exts), ext) != std::end(exts);
}

TextFile::TextFile(std::string fileName):
    fileName_(getFullFileName(fileName))
{
    if (!fileName.empty())
        setName(toUtf16(baseName(fileName)));
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
    if (isCpp(fileName))
        highlighter_ = new CppHighlighter(this);
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
    setName(toUtf16(baseName(fileName)));
    std::ofstream f(fileName_);
    bool first = true;
    for (const auto &l: buffer_)
    {
        if (first)
            first = false;
        else
            f << std::endl;
        f << toUtf8(l);
    }
    clearModified();
}


std::wstring TextFile::preInsert(Coord &cursor, std::wstring value)
{
    if (value.size() != 1 || value[0] != '\n')
        return value;
    else
    {
        auto &line = (*this)[cursor.y];
        std::wstring spaces;
        int c = 0;
        for (auto ch: line)
            if (ch == L' ')
                ++c;
            else
                break;
        for (size_t x = cursor.x; x < line.size() && line[x] == ' '; ++x, --c);
        for (int i = 0; i < c; ++i)
            spaces += L' ';
        return L'\n' + spaces;
    }
}
