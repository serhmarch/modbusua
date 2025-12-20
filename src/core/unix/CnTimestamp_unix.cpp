#include <CnTimestamp.h>

#include "CnUnix.h"

static uint64_t toLocalTime(uint64_t utcTime)
{
    // TODO: check
    uint64_t rem = utcTime % CN_NANOSEC100_IN_SEC;
    time_t utcsec = utcTime / CN_NANOSEC100_IN_SEC - CN_SEC_BETWEEN_1601_AND_1970; // Convert to seconds
    struct tm localTm;
    localtime_r(&utcsec, &localTm);
    time_t localTime = timegm(&localTm); // Get local timestamp

    // Convert back to 100-ns intervals
    return (static_cast<uint64_t>(localTime)+CN_SEC_BETWEEN_1601_AND_1970) * CN_NANOSEC100_IN_SEC + rem;
}

static uint64_t toUtcTime(uint64_t localTime)
{
    // TODO: check
    uint64_t rem = localTime % CN_NANOSEC100_IN_SEC;

    // Convert 100-ns intervals to Unix timestamp
    time_t localsec = localTime / CN_NANOSEC100_IN_SEC - CN_SEC_BETWEEN_1601_AND_1970;

    struct tm localTm;
    gmtime_r(&localsec, &localTm); // Convert local time to UTC
    time_t utcTime = mktime(&localTm); // Get UTC timestamp

    // Convert back to 100-ns intervals
    return (static_cast<uint64_t>(utcTime)+CN_SEC_BETWEEN_1601_AND_1970) * CN_NANOSEC100_IN_SEC + rem;
}


CnTimestamp CnTimestamp::current()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return CnTimestamp(static_cast<uint64_t>(ts.tv_sec) * CN_NANOSEC100_IN_SEC
                     + ts.tv_nsec / CN_NANOSEC_IN_NANOSEC100
                     + CN_NANOSEC100_BETWEEN_1601_AND_1970);
}

CnTimestamp CnTimestamp::currentPrecise()
{
    // On Unix, `current()` is already precise enough.
    return current();
}

CnTimestamp::CnTimestamp(uint64_t rawValue, bool local)
{
    if (local)
    {
        m_data = toUtcTime(rawValue);
    }
    else
        m_data = rawValue;
}

uint64_t CnTimestamp::toLocalRawValue() const
{
    return toLocalTime(m_data);
}

uint64_t CnTimestamp::toLocalMSecSinceEpoch() const
{
    uint64_t localMSec100Since1601 = toLocalTime(m_data) / CN_NANOSEC100_IN_MILLISEC;
    uint64_t localMSecSince1970 = localMSec100Since1601 - CN_MSEC_BETWEEN_1601_AND_1970;
    return localMSecSince1970;
}

uint64_t CnTimestamp::toLocalMSec1980() const
{
    uint64_t localMSec100Since1601 = toLocalTime(m_data) / CN_NANOSEC100_IN_MILLISEC;
    uint64_t localMSecSince1970 = localMSec100Since1601 - CN_MSEC_BETWEEN_1601_AND_1980;
    return localMSecSince1970;
}
