#include <CnTimestamp.h>

#include "CnWindows.h"

CnTimestamp CnTimestamp::current()
{
    Cn::ftconv c;
    GetSystemTimeAsFileTime(&c.ft);
    return CnTimestamp(c.data);
}

CnTimestamp CnTimestamp::currentPrecise()
{
    Cn::ftconv c;
    GetSystemTimePreciseAsFileTime(&c.ft);
    return CnTimestamp(c.data);
}

CnTimestamp::CnTimestamp(uint64_t rawValue, bool local)
{
    if (local)
    {
        Cn::ftconv c, cUtc;
        c.data = rawValue;
        LocalFileTimeToFileTime(&c.ft, &cUtc.ft);
        m_data = cUtc.data;
    }
    else
        m_data = rawValue;
}

uint64_t CnTimestamp::toLocalRawValue() const
{
    uint64_t local;
    FileTimeToLocalFileTime(reinterpret_cast<const FILETIME*>(&m_data), reinterpret_cast<FILETIME*>(&local));
    return local;
}

uint64_t CnTimestamp::toLocalMSecSinceEpoch() const
{
    uint64_t local;
    FileTimeToLocalFileTime(reinterpret_cast<const FILETIME*>(&m_data), reinterpret_cast<FILETIME*>(&local));
    return (local / CN_NANOSEC100_IN_MILLISEC) - CN_MSEC_BETWEEN_1601_AND_1970;
}

uint64_t CnTimestamp::toLocalMSec1980() const
{
    uint64_t local;
    FileTimeToLocalFileTime(reinterpret_cast<const FILETIME*>(&m_data), reinterpret_cast<FILETIME*>(&local));
    return (local / CN_NANOSEC100_IN_MILLISEC) - CN_MSEC_BETWEEN_1601_AND_1980;
}
