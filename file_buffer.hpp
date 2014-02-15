#pragma once
#include "base_text_buffer.hpp"

class FileBuffer: public BaseTextBuffer
{
public:
    FileBuffer(std::string fileName);
private:
    std::string fileName_;
};
