#ifndef CNSTD_IOSTREAM_H
#define CNSTD_IOSTREAM_H

#include <CnConfig.h>

#include <iostream>

namespace CnStd {

#ifdef CN_USE_WSTRING

inline auto& cin = std::wcin;
inline auto& cout = std::wcout;
inline auto& cerr = std::wcerr;

#else

inline auto& cin = std::cin;
inline auto& cout = std::cout;
inline auto& cerr = std::cerr;

#endif // CN_USE_WSTRING

} // namespace CnStd

#endif // CNSTD