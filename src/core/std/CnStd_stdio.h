#ifndef CNSTD_STDIO_H
#define CNSTD_STDIO_H

#include <CnConfig.h>
#include <CnCore.h>

#include <stdarg.h>
#include <stdio.h>
#include <cwchar>

namespace CnStd {


inline int vsnprintf(CnChar* buffer, std::size_t buf_size, const CnChar* format, va_list vlist)
{
#ifdef CN_USE_WSTRING
    return std::vswprintf(buffer, buf_size, format, vlist);
#else
    return std::vsnprintf(buffer, buf_size, format, vlist);
#endif // CN_USE_WSTRING
}

inline int snprintf(CnChar* buffer, std::size_t buf_size, const CnChar* format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    auto res = CnStd::vsnprintf(buffer, buf_size, format, vlist);
    va_end(vlist);
    return res;
}

} // namespace CnStd

#endif // CNSTD_STDIO_H