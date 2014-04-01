#pragma once
#include "base_text_buffer.hpp"

class TextFile: public BaseTextBuffer
{
public:
    TextFile(std::string fileName = "");
    std::string fileName() const;
    void save();
    void saveAs(std::string fileName);
private:
    std::string fileName_;
    std::wstring preInsert(Coord &cursor, std::wstring);
};
