#include <CnDateTime.h>

#include <CnStd_sstream.h>

#include "CnDateTime_p_unix.h"

inline CnDateTimePrivateUnix *d_cast(CnDateTimePrivate *d) { return static_cast<CnDateTimePrivateUnix*>(d); }

static CnDateTimePrivateUnix nullCnDateTimePrivateUnix;

CnDateTimePrivate* getNullCnDateTimePrivate() { return &nullCnDateTimePrivateUnix; }

CnDateTimePrivate* createCnDateTimePrivate() { return new CnDateTimePrivateUnix(); }

CnDateTimePrivate* createCnDateTimePrivate(const CnDateTimePrivate* other) { return new CnDateTimePrivateUnix(*static_cast<const CnDateTimePrivateUnix*>(other)); }

CnDateTime CnDateTime::currentUtcDateTime()
{
    CnDateTime dateTime(0, 0, 0);
    CnDateTimePrivateUnix *d = d_cast(dateTime.d);
    d->flags = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    gmtime_r(&tv.tv_sec, &d->dt);
    d->dt_milliseconds = tv.tv_usec / CN_MICROSEC_IN_MILLISEC;
    return dateTime;
}

CnDateTime CnDateTime::currentLocalDateTime()
{
    CnDateTime dateTime(0, 0, 0);
    CnDateTimePrivateUnix *dd = d_cast(dateTime.d);
    dd->flags = CnDateTimePrivate::Flag_Local;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &dd->dt);
    dd->dt_milliseconds = tv.tv_usec / CN_MICROSEC_IN_MILLISEC;
    return dateTime;
}

CnDateTime::CnDateTime(uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t millisecond, bool local)
{
    CnDateTimePrivateUnix *dd = new CnDateTimePrivateUnix();
    dd->flags = local ? CnDateTimePrivate::Flag_Local : 0;
    dd->dt.tm_isdst = -1; // Let the system determine whether daylight saving time is in effect
    dd->dt.tm_year = year - 1900;
    dd->dt.tm_mon = month - 1;
    dd->dt.tm_mday = day;
    dd->dt.tm_hour = hour;
    dd->dt.tm_min = minute;
    dd->dt.tm_sec = second;
    dd->dt_milliseconds = millisecond;
    // Normalize the time structure
    if (local)
        mktime(&dd->dt);
    else
        timegm(&dd->dt);
    d = dd;
}

CnDateTime::CnDateTime(const CnTimestamp &timestamp, bool local)
{
    CnDateTimePrivateUnix *dd = new CnDateTimePrivateUnix();
    dd->flags = local ? CnDateTimePrivate::Flag_Local : 0;
    auto msec = timestamp.toMSecSinceEpoch();
    time_t rawTime = msec / CN_MILLISEC_IN_SEC;
    dd->dt_milliseconds = msec % CN_MILLISEC_IN_SEC; // Extract remaining milliseconds
    if (local)
        localtime_r(&rawTime, &dd->dt);
    else
        gmtime_r(&rawTime, &dd->dt);
    d = dd;
}

uint16_t CnDateTime::year() const 
{ 
    return d_cast(d)->year(); 
}

uint16_t CnDateTime::month() const       
{
    return d_cast(d)->month();
}

uint16_t CnDateTime::day() const         
{
    return d_cast(d)->day();
}

uint16_t CnDateTime::hour() const        
{ 
    return d_cast(d)->hour();
}

uint16_t CnDateTime::minute() const      
{
    return d_cast(d)->minute();
}

uint16_t CnDateTime::second() const      
{
    return d_cast(d)->second();
}

uint16_t CnDateTime::milliseconds() const
{
    return d_cast(d)->milliseconds();
}

uint16_t CnDateTime::dayOfWeek() const   
{
    return d_cast(d)->dayOfWeek();
}

CnDateTime CnDateTime::toLocal() const
{
    if (d != &nullCnDateTimePrivateUnix)
    {
        CnDateTimePrivateUnix *dd = d_cast(d);
        if (dd->isLocal())
            return *this;
        CnDateTime dtReturn(0, 0, 0);
        CnDateTimePrivateUnix *dr = d_cast(dtReturn.d);
        dr->flags = CnDateTimePrivate::Flag_Local;
        time_t rawTime = timegm(&dd->dt);
        if (rawTime == -1)
        {
            dtReturn.clear();
        }
        else
        {
            localtime_r(&rawTime, &dr->dt);
            dr->dt_milliseconds = dd->dt_milliseconds;
        }
        return dtReturn;
    }
    return CnDateTime();
}

CnDateTime CnDateTime::toUtc() const
{
    if (d != &nullCnDateTimePrivateUnix)
    {
        CnDateTimePrivateUnix *dd = d_cast(d);
        if (dd->isUtc())
            return *this;
        CnDateTime dtReturn(0, 0, 0);
        dtReturn.d->flags = 0;
        time_t rawTime = mktime(&dd->dt);
        if (rawTime == -1)
        {
            dtReturn.clear();
        }
        else
        {
            gmtime_r(&rawTime, &d_cast(dtReturn.d)->dt);
            d_cast(dtReturn.d)->dt_milliseconds = dd->dt_milliseconds;
        }
        return dtReturn;
    }
    return CnDateTime();
}

CnTimestamp CnDateTime::toTimestamp() const
{
    if (d != &nullCnDateTimePrivateUnix)
    {
        CnDateTimePrivateUnix *dd = d_cast(d);
        time_t rawTime;
        if (dd->isLocal())
            rawTime = mktime(&dd->dt);
        else
            rawTime = timegm(&dd->dt);
        if (rawTime == -1)
            return CnTimestamp();
        return CnTimestamp(static_cast<uint64_t>(rawTime) * CN_NANOSEC100_IN_SEC
                            + dd->dt_milliseconds * CN_NANOSEC100_IN_MILLISEC
                            + CN_NANOSEC100_BETWEEN_1601_AND_1970);
    }
    return CnTimestamp();
}

void CnDateTime::clear()
{
    CNDATETIME_DETACH
    d_cast(d)->clear();
}

void CnDateTime::clearTimeOnly()
{
    if (d != &nullCnDateTimePrivateUnix)
    {
        CNDATETIME_DETACH
        CnDateTimePrivateUnix *dd = d_cast(d);
        dd->dt_milliseconds = 0;
        dd->dt.tm_hour = 0;
        dd->dt.tm_min = 0;
        dd->dt.tm_sec = 0;
    }
}
