#ifndef CNSTD_SSTREAM_H
#define CNSTD_SSTREAM_H

#include <CnConfig.h>

#include <sstream>
#include <fstream>
#include <iomanip>

namespace CnStd {

#ifdef CN_USE_WSTRING

using ios           = std::wios          ;
using streambuf     = std::wstreambuf    ;
using istream       = std::wistream      ;
using ostream       = std::wostream      ;
using iostream      = std::wiostream     ;
using stringbuf     = std::wstringbuf    ;
using istringstream = std::wistringstream;
using ostringstream = std::wostringstream;
using stringstream  = std::wstringstream ;
using filebuf       = std::wfilebuf      ;
using ifstream      = std::wifstream     ;
using ofstream      = std::wofstream     ;
using fstream       = std::wfstream      ;

#else

using ios           = std::ios          ;
using streambuf     = std::streambuf    ;
using istream       = std::istream      ;
using ostream       = std::ostream      ;
using iostream      = std::iostream     ;
using stringbuf     = std::stringbuf    ;
using istringstream = std::istringstream;
using ostringstream = std::ostringstream;
using stringstream  = std::stringstream ;
using filebuf       = std::filebuf      ;
using ifstream      = std::ifstream     ;
using ofstream      = std::ofstream     ;
using fstream       = std::fstream      ;

#endif // CN_USE_WSTRING

} // namespace CnStd

#endif // CNSTD_SSTREAM_H