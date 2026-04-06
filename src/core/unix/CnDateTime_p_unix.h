#ifndef CNDATETIME_P_UNIX_H
#define CNDATETIME_P_UNIX_H

#include <CnDateTime_p.h>

#include "CnUnix.h"

class CnDateTimePrivateUnix : public CnDateTimePrivate
{
public:
    CnDateTimePrivateUnix() : CnDateTimePrivate()
    {
        memset(&this->dt, 0, sizeof(this->dt));
        dt_milliseconds = 0;
    }

    CnDateTimePrivateUnix(const CnDateTimePrivateUnix &other) : CnDateTimePrivate(other)
    {
        memcpy(&this->dt, &other.dt, sizeof(this->dt));
        dt_milliseconds = other.dt_milliseconds;
    }

    ~CnDateTimePrivateUnix() override
    {
        auto dummy = 0;
    }


public:
    inline uint16_t year() const { return dt.tm_year+1900; }
    inline uint16_t month() const { return dt.tm_mon+1; }
    inline uint16_t day() const { return dt.tm_mday; }
    inline uint16_t hour() const { return dt.tm_hour; }
    inline uint16_t minute() const { return dt.tm_min; }
    inline uint16_t second() const { return dt.tm_sec; }
    inline uint16_t milliseconds() const { return dt_milliseconds; }
    inline uint16_t dayOfWeek() const { return dt.tm_wday; }
    inline void clear() { memset(&dt, 0, sizeof(dt)); dt_milliseconds = 0;}

public:
    struct tm dt;
    int dt_milliseconds;
};

#endif // CNDATETIME_P_UNIX_H