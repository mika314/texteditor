#include "current_dir.hpp"
#include "to_utf16.hpp"
#include <unistd.h>
#include <sys/param.h>
#include <cstdlib>

std::string getCurrentDir()
{
    char *tmp = getcwd(nullptr, MAXPATHLEN);
    std::string result = tmp;
    free(tmp);
    return result;
}
