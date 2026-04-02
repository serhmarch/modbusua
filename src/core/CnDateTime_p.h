#ifndef CNDATETIME_P_H
#define CNDATETIME_P_H

#include "CnDateTime.h"

class CnDateTimePrivate
{
public:
    inline static bool checkDateTime(uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t milliseconds) 
    {     
        return (month >= 1 && month <= 12) &&
               (day >= 1 && day <= 31) &&
               (hour >= 0 && hour <= 23) &&
               (minute >= 0 && minute <= 59) &&
               (second >= 0 && second <= 59) &&
               (milliseconds >= 0 && milliseconds <= 999);
    }

    enum Flags
    {
        Flag_Local = 0x01
    };

public:
    CnDateTimePrivate() :
        ref(1),
        flags(0)
    { 
    }

    CnDateTimePrivate(const CnDateTimePrivate &other) :
        ref(1),
        flags(other.flags)
    { 
    }

    virtual ~CnDateTimePrivate()
    {
    }

public:
    inline void incRef() { ++ref; }
    inline void decRef() { if (--ref < 1) delete this; }

public:
    inline bool isUtc() const { return !isLocal(); }
    inline bool isLocal() const { return (flags & Flag_Local); }

public:
    CnRefCount_t ref;
    int flags;
};

CnDateTimePrivate* getNullCnDateTimePrivate();
CnDateTimePrivate* createCnDateTimePrivate();
CnDateTimePrivate* createCnDateTimePrivate(const CnDateTimePrivate* other);

#define CNDATETIME_DETACH \
    if (d->ref > 1) { \
        CnDateTimePrivate* dd = createCnDateTimePrivate(d); \
        d->decRef(); \
        d = dd; \
    }

#endif // CNDATETIME_P_H