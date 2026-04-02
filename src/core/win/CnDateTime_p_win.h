#ifndef CNDATETIME_P_WIN_H
#define CNDATETIME_P_WIN_H

#include <CnDateTime_p.h>

#include "CnWindows.h"

class CnDateTimePrivateWin : public CnDateTimePrivate
{
public:
    CnDateTimePrivateWin() : CnDateTimePrivate()
    {
        memset(&this->dt, 0, sizeof(this->dt));
    }

    CnDateTimePrivateWin(const CnDateTimePrivateWin &other) : CnDateTimePrivate(other)
    {
        this->dt = other.dt;
    }

    ~CnDateTimePrivateWin() override = default;

public:
    inline uint16_t year() const { return dt.wYear; }
    inline uint16_t month() const { return dt.wMonth; }
    inline uint16_t day() const { return dt.wDay; }
    inline uint16_t hour() const { return dt.wHour; }
    inline uint16_t minute() const { return dt.wMinute; }
    inline uint16_t second() const { return dt.wSecond; }
    inline uint16_t milliseconds() const { return dt.wMilliseconds; }
    inline uint16_t dayOfWeek() const { return dt.wDayOfWeek; }
    inline void clear() { memset(&dt, 0, sizeof(dt)); }

public:
    SYSTEMTIME dt;
};

inline CnDateTimePrivateWin *d_win(CnDateTimePrivate *d) { return static_cast<CnDateTimePrivateWin*>(d); }

#endif // CNDATETIME_P_WIN_H