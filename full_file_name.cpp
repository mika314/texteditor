#include "full_file_name.hpp"
#include "current_dir.hpp"

std::string getFullFileName(const std::string &fileName)
{
  if (fileName.empty() || fileName[0] == L'/') 
    return fileName;
  else 
    return getCurrentDir() + '/' + fileName;
}
