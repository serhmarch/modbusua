#ifndef CNSTD_STRING_H
#define CNSTD_STRING_H

#define __STDC_WANT_LIB_EXT1__ 1

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996) // Note: to suppress warnings about deprecated functions like strcpy.
#endif

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <string>

#include <CnCore.h>

namespace CnStd {

#ifdef CN_USE_WSTRING
using string = std::wstring;
#else
using string = std::string;
#endif

inline size_t strlen(const CnChar* str) 
{
#ifdef CN_USE_WSTRING
    return ::wcslen(str);
#else
    return ::strlen(str);
#endif
}

inline CnChar* strcpy(CnChar* dest, const CnChar* src) 
{
#ifdef CN_USE_WSTRING
    return ::wcscpy(dest, src);
#else
    return ::strcpy(dest, src);
#endif
}

int strcpy_s(CnChar* dest, size_t destsz, const CnChar* src);

inline CnChar* strncpy(CnChar* dest, const CnChar* src, size_t count) 
{
#ifdef CN_USE_WSTRING
    return ::wcsncpy(dest, src, count);
#else
    return ::strncpy(dest, src, count);
#endif
}

int strncpy_s(CnChar* dest, size_t destsz, const CnChar* src, size_t count);

inline const CnChar* strchr(const CnChar* str, int ch)
{
#ifdef CN_USE_WSTRING
    return ::wcschr(str, ch);
#else
    return ::strchr(str, ch);
#endif
}

inline long strtol(const CnChar* str, CnChar** str_end, int base) 
{
#ifdef CN_USE_WSTRING
    return ::wcstol(str, str_end, base);
#else
    return ::strtol(str, str_end, base);
#endif
}

inline unsigned long strtoul(const CnChar* str, CnChar** str_end, int base) 
{
#ifdef CN_USE_WSTRING
    return ::wcstoul(str, str_end, base);
#else
    return ::strtoul(str, str_end, base);
#endif
}

inline long long strtoll(const CnChar* str, CnChar** str_end, int base) 
{
#ifdef CN_USE_WSTRING
    return ::wcstoll(str, str_end, base);
#else
    return ::strtoll(str, str_end, base);
#endif
}

inline long long strtoull(const CnChar* str, CnChar** str_end, int base) 
{
#ifdef CN_USE_WSTRING
    return ::wcstoull(str, str_end, base);
#else
    return ::strtoull(str, str_end, base);
#endif
}

inline float strtof(const CnChar* str, CnChar** str_end) 
{
#ifdef CN_USE_WSTRING
    return ::wcstof(str, str_end);
#else
    return ::strtof(str, str_end);
#endif
}

inline double strtod(const CnChar* str, CnChar** str_end) 
{
#ifdef CN_USE_WSTRING
    return ::wcstod(str, str_end);
#else
    return ::strtod(str, str_end);
#endif
}

inline int strcmp(const CnChar* lhs, const CnChar* rhs) 
{
#ifdef CN_USE_WSTRING
    return ::wcscmp(lhs, rhs);
#else
    return ::strcmp(lhs, rhs);
#endif
}

inline int strncmp(const CnChar* lhs, const CnChar* rhs, std::size_t count) 
{
#ifdef CN_USE_WSTRING
    return ::wcsncmp(lhs, rhs, count);
#else
    return ::strncmp(lhs, rhs, count);
#endif
}

} // namespace CnStd

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // CNSTD_STRING_H