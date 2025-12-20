#include "CnTimestamp.h"

CnTimestamp CnTimestamp::fromUtcMSecSinceEpoch(int64_t msec)
{
    CnTimestamp tm;
    tm.m_data = (msec+CN_MSEC_BETWEEN_1601_AND_1970) * CN_NANOSEC100_IN_MILLISEC;
    return tm;
}

CnTimestamp::CnTimestamp()
{
    m_data = 0;
}

CnTimestamp::CnTimestamp(const CnTimestamp & other)
{
    m_data = other.m_data;
}

uint64_t CnTimestamp::toUtcMSecSinceEpoch() const
{
    uint64_t msecSince1601 = milliseconds();
    uint64_t msecSince1970 = msecSince1601 - CN_MSEC_BETWEEN_1601_AND_1970;
    return msecSince1970;
}

uint64_t CnTimestamp::toUtcMSec1980() const
{
    uint64_t msecSince1601 = milliseconds();
    uint64_t msecSince1980 = msecSince1601 - CN_MSEC_BETWEEN_1601_AND_1980;
    return msecSince1980;
}
