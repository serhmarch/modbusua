#ifndef CNUNIX_H
#define CNUNIX_H

#include <CnCore.h>
#include <CnString.h>
#include <CnTimestamp.h>

#include <string.h>
#include <unistd.h>
#include <sys/time.h>

namespace Cn {

inline CnTimestamp toTimestamp(time_t tm) { return CnTimestamp((tm+CN_SEC_BETWEEN_1601_AND_1970)*CN_NANOSEC100_BETWEEN_1601_AND_1970); }
inline CnTimestamp toTimestamp(timespec tm) { return CnTimestamp((tm.tv_sec+CN_SEC_BETWEEN_1601_AND_1970)*CN_NANOSEC100_BETWEEN_1601_AND_1970+
                                                                  tm.tv_nsec/CN_NANOSEC_IN_NANOSEC100); }

inline mode_t to_mode_t(int permissions) { return static_cast<mode_t>(permissions); }

} // namespace Cn

#endif // CNUNIX_H