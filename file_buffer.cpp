#include "file_buffer.hpp"
#include "to_utf16.hpp"
#include <fstream>

FileBuffer::FileBuffer(std::string fileName):
    fileName_(fileName)
{
    std::ifstream f(fileName_);
    while (!f.eof())
    {
        std::string line;
        std::getline(f, line);
        buffer_.push_back(toUtf16(line));
    }
}
