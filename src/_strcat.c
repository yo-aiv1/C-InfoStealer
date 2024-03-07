#include <stdlib.h>
#include <string.h>

#include "../include/_strcat.h"


char *_strcat(const char *str1, const char *str2)
{
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    char *ResultStr = malloc(len1 + len2 + 1);
    if (!ResultStr)
    {
        return NULL;
    }
    memcpy(ResultStr, str1, len1);
    memcpy(ResultStr + len1, str2, len2 + 1);
    return ResultStr;
}