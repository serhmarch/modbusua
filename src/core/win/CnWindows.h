#ifndef CNWINDOWS_H
#define CNWINDOWS_H

#include <CnCore.h>

namespace Cn {

union ftconv
{
    FILETIME ft;
    uint64_t data;
};

inline CnTimestamp toTimestamp(time_t tm) { return CnTimestamp((tm+CN_SEC_BETWEEN_1601_AND_1970)*CN_NANOSEC100_BETWEEN_1601_AND_1970); }

} // namespace Cn

#endif // CNWINDOWS_H