#include <CnDateTime.h>

#include <CnStd_sstream.h>

#include "CnDateTime_p_win.h"

// TODO: check SYSTEMTIME and FILETIME structures for correctness

inline CnDateTimePrivateWin *d_cast(CnDateTimePrivate *d) { return static_cast<CnDateTimePrivateWin*>(d); }

static CnDateTimePrivateWin nullCnDateTimePrivateWin;

CnDateTimePrivate* getNullCnDateTimePrivate() { return &nullCnDateTimePrivateWin; }

CnDateTimePrivate* createCnDateTimePrivate() { return new CnDateTimePrivateWin(); }

CnDateTimePrivate* createCnDateTimePrivate(const CnDateTimePrivate* other) { return new CnDateTimePrivateWin(*static_cast<const CnDateTimePrivateWin*>(other)); }

CnDateTime CnDateTime::currentUtcDateTime()
{
    CnDateTime dateTime(0, 0, 0);
    CnDateTimePrivateWin *dd = d_cast(dateTime.d);
    dd->flags = 0;
    GetSystemTime(&dd->dt);
    return dateTime;
}

CnDateTime CnDateTime::currentLocalDateTime()
{
    CnDateTime dateTime(0, 0, 0);
    CnDateTimePrivateWin *dd = d_cast(dateTime.d);
    dd->flags = CnDateTimePrivate::Flag_Local;
    GetLocalTime(&dd->dt);
    return dateTime;
}

CnDateTime::CnDateTime(uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t millisecond, bool local)
{
    CnDateTimePrivateWin *dd = new CnDateTimePrivateWin();
    dd->flags = local ? CnDateTimePrivate::Flag_Local : 0;
    dd->dt.wYear         = year       ;
    dd->dt.wMonth        = month      ;
    dd->dt.wDay          = day        ;
    dd->dt.wHour         = hour       ;
    dd->dt.wMinute       = minute     ;
    dd->dt.wSecond       = second     ;
    dd->dt.wMilliseconds = millisecond;
    FILETIME ft;
    if (!SystemTimeToFileTime(&dd->dt, &ft))
        dd->clear();
    if (!FileTimeToSystemTime(&ft, &dd->dt))
        dd->clear();
    d = dd;
}

CnDateTime::CnDateTime(const CnTimestamp &timestamp, bool local)
{
    CnDateTimePrivateWin *dd = new CnDateTimePrivateWin();
    Cn::ftconv c;
    c.data = timestamp.rawValue(); // UTC timestamp raw value
    bool ok;
    if (local)
    {
        SYSTEMTIME utcDt;
        ok = FileTimeToSystemTime(&c.ft, &utcDt) != 0; // Convert to SYSTEMTIME in UTC
        if (ok)
            ok = SystemTimeToTzSpecificLocalTime(NULL, &utcDt, &dd->dt) != 0;
        dd->flags = CnDateTimePrivate::Flag_Local;
    }
    else
    {
        ok = FileTimeToSystemTime(&c.ft, &dd->dt) != 0;
        dd->flags = 0;
    }
    if (!ok)
        dd->clear();
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
    if (d != &nullCnDateTimePrivateWin)
    {
        CnDateTimePrivateWin *dd = d_cast(d);
        if (dd->isLocal())
            return *this;
        CnDateTime dtReturn(0, 0, 0);
        dtReturn.d->flags = CnDateTimePrivate::Flag_Local;
        if (!SystemTimeToTzSpecificLocalTime(NULL, &dd->dt, &d_cast(dtReturn.d)->dt))
            dtReturn.clear();
        return dtReturn;
    }
    return CnDateTime();
}

CnDateTime CnDateTime::toUtc() const
{
    if (d != &nullCnDateTimePrivateWin)
    {
        CnDateTimePrivateWin *dd = d_cast(d);
        if (dd->isUtc())
            return *this;
        CnDateTime dtReturn(0, 0, 0);
        dtReturn.d->flags = 0;
        if (!TzSpecificLocalTimeToSystemTime(NULL, &dd->dt, &d_cast(dtReturn.d)->dt))
            dtReturn.clear();
        return dtReturn;
    }
    return CnDateTime();
}

CnTimestamp CnDateTime::toTimestamp() const
{
    if (d != &nullCnDateTimePrivateWin)
    {
        CnDateTime utc = toUtc();
        Cn::ftconv c;
        if (!SystemTimeToFileTime(&d_cast(utc.d)->dt, &c.ft))
            return CnTimestamp();
        return CnTimestamp(c.data);
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
    if (d != &nullCnDateTimePrivateWin)
    {
        CNDATETIME_DETACH
        CnDateTimePrivateWin *dd = d_cast(d);
        dd->dt.wMilliseconds = 0;
        dd->dt.wHour = 0;
        dd->dt.wMinute = 0;
        dd->dt.wSecond = 0;
    }
}