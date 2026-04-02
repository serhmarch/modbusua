#include "CnDateTime.h"
#include "CnDateTime_p.h"

#include <CnStd_sstream.h>

CnDateTime::CnDateTime() : 
    d(getNullCnDateTimePrivate())
{
    d->incRef();
}

CnDateTime::CnDateTime(const CnDateTime &other)
{
    if (this != &other)
    {
        other.d->incRef();
        d = other.d;
    }
}

CnDateTime::CnDateTime(CnDateTime &&other)
{
    auto* dd = getNullCnDateTimePrivate();
    dd->incRef();
    d = other.d;
    other.d = dd;
}

CnDateTime::~CnDateTime()
{
    d->decRef();
}

bool CnDateTime::isNull() const
{
    return d == getNullCnDateTimePrivate();
}

bool CnDateTime::isLocal() const
{
    return d->isLocal();
}

bool CnDateTime::isUtc() const
{
    return d->isUtc();
}

void CnDateTime::swap(CnDateTime &other)
{
    CnDateTimePrivate *old = d;
    d = other.d;
    other.d = old;
}

CnDateTime &CnDateTime::operator=(const CnDateTime &other)
{
    if (this != &other)
    {
        other.d->incRef();
        d->decRef();
        d = other.d;
    }
    return *this;
}

CnDateTime &CnDateTime::operator=(CnDateTime &&other)
{
    if (this != &other)
    {
        auto* dd = getNullCnDateTimePrivate();
        dd->incRef();
        d->decRef();
        d = other.d;
        other.d = dd;
    }
    return *this;
}

void CnDateTime::addMilliseconds(int64_t milliseconds)
{
    // TODO: optimize function
    CnTimestamp tm = this->toTimestamp(); // Note: 'CnTimestamp' always has Utc format
    tm.addMilliseconds(milliseconds);
    CnDateTime other(tm);
    if (this->isLocal())
    {
        other = other.toLocal();
        this->swap(other);
    }
    else
    {
        this->swap(other);
    }
}

CnString CnDateTime::toString(const CnString &format) const
{
    if (d != getNullCnDateTimePrivate())
    {
        CnStd::ostringstream result;

        for (size_t i = 0; i < format.length(); ++i) 
        {
            if (format[i] == CnCHR('%')) 
            {
                if (i + 1 < format.length()) 
                {
                    CnChar specifier = format[i + 1];
                    switch (specifier) 
                    {
                    case CnCHR('Y'):
                        result << this->year();
                        break;
                    case CnCHR('M'):
                    {
                        auto v = this->month();
                        if (v < 10)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    case CnCHR('D'):
                    {
                        auto v = this->day();
                        if (v < 10)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    case CnCHR('h'):
                    {
                        auto v = this->hour();
                        if (v < 10)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    case CnCHR('m'):
                    {
                        auto v = this->minute();
                        if (v < 10)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    case CnCHR('s'):
                    {
                        auto v = this->second();
                        if (v < 10)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    case CnCHR('f'):
                    {
                        auto v = this->milliseconds();
                        if (v < 10)
                            result << CnSTR("00");
                        else if (v < 100)
                            result << CnCHR('0');
                        result << v;
                    }
                        break;
                    default:
                        result << CnCHR('%');
                        result << specifier;
                        break;
                    }
                    ++i;
                }
                else 
                {
                    result << CnCHR('%');
                }
            }
            else 
            {
                result << format[i];
            }
        }
        return CnString(result.str().data());
    }
    return CnString();
}
