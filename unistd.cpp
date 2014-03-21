#include "unistd.h"
#include <cstdlib>

char *getcwd(char *, int maxPathLen)
{
    char *res = static_cast<char *>(malloc(maxPathLen));
    res[0] = '\0';
    return res;
}

void chdir(const char *value)
{
}
