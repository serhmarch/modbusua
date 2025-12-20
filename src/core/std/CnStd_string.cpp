#include "CnStd_string.h"

#include <cerrno>

namespace CnStd {

int strcpy_s(CnChar* dest, size_t destsz, const CnChar* src)
{
    if (!dest || !src || destsz == 0)
        return EINVAL;
    size_t i = 0;
    // Use standard types for error codes
    for (; i < destsz - 1 && src[i] != '\0'; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    // If src was longer than destsz-1, return range error
    if (src[i] != '\0')
        return ERANGE;
    return 0;
}

int strncpy_s(CnChar* dest, size_t destsz, const CnChar* src, size_t count)
{
    if (!dest || !src || destsz == 0 || count == 0)
        return EINVAL;
    size_t i = 0;
    // Use standard types for error codes
    for (; i < destsz - 1 && i < count; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    return 0;
}

} // namespace CnStd 
