/*! 
 * \file   CnDateTime.h
 * \brief  Contains the description of the date/time class
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDATETIME_H
#define CNDATETIME_H

#include "CnTimestamp.h"
#include "CnString.h"

class CnDateTimePrivate;

/*! \brief The `CnDateTime` class is designed for convenient work with date and time

    \details `CnDateTime` is used for working with date and time.\n
    It has functions and methods for converting individual variables such as year, month, day, hour, minute, second, and milliseconds
    to the CnDateTime format, CnTimestamp, as well as performing reverse conversions.\n
    Date and time can be stored in Local or UTC format.
*/

class CnDateTime
{
public:
    /// \details Returns the current date and time in UTC format.
    static CnDateTime currentUtcDateTime();

    /// \details Returns the current date and time in local format.
    static CnDateTime currentLocalDateTime();

    /// \details Returns the current date and time. If the `local=false` parameter (default), the date/time is in UTC format; if `local=true`, it is in local format.
    inline static CnDateTime currentDateTime(bool local = false) { return local ? currentLocalDateTime() : currentUtcDateTime();  }

    /// \details Returns the current timestamp.
    inline static CnTimestamp currentTimestamp() { return CnTimestamp::current(); }

    /// \details Returns the current high-precision timestamp using the WinAPI function `GetSystemTimePreciseAsFileTime`.
    inline static CnTimestamp currentTimestampPrecise() { return CnTimestamp::currentPrecise(); }

    /// \details Returns the current timestamp. Same as `CnDateTime::currentTimestamp()`.
    inline static CnTimestamp currentUtcTimestamp() { return currentTimestamp(); }

    /// \details Returns a date and time object in UTC format, created from individual variables `year`, `month`, `day`, `hour`, `minute`, `second`, `millisecond`.
    /// The variables `hour`, `minute`, `second`, `millisecond` can be omitted. In that case, `0` is substituted for them.
    inline static CnDateTime utc(uint16_t year, uint16_t month, uint16_t day,
                               uint16_t hour = 0,
                               uint16_t minute = 0,
                               uint16_t second = 0,
                               uint16_t millisecond = 0) { return CnDateTime(year, month, day, hour, minute, second, millisecond); }

    /// \details Returns a date and time object in UTC format, created from the timestamp `CnTimestamp`.
    inline static CnDateTime utc(const CnTimestamp& t) { return CnDateTime(t); }

    /// \details Returns a date and time object in local format, created from individual variables `year`, `month`, `day`, `hour`, `minute`, `second`, `millisecond`.
    /// The variables `hour`, `minute`, `second`, `millisecond` can be omitted. In that case, `0` is substituted for them.
    inline static CnDateTime local(uint16_t year, uint16_t month, uint16_t day,
                                 uint16_t hour = 0,
                                 uint16_t minute = 0,
                                 uint16_t second = 0,
                                 uint16_t millisecond = 0) { return CnDateTime(year, month, day, hour, minute, second, millisecond, true); }

    /// \details Returns a date and time object in local format, created from the timestamp `CnTimestamp`.
    inline static CnDateTime local(const CnTimestamp& t) { return CnDateTime(t, true); }

public:
    /// \details The constructor creates an empty (`null`) date and time.
    CnDateTime();

    /// \details Copy constructor. Creates a copy of the `other` object.
    CnDateTime(const CnDateTime &other);

    /// \details Move constructor. Moves the data from the `other` object to the current object.
    CnDateTime(CnDateTime &&other);

    /// \details Constructor creates a date and time object specified by individual variables `year`, `month`, `day`, `hour`, `minute`, `second`, `millisecond`.
    /// The variables `hour`, `minute`, `second`, `millisecond` can be omitted. In that case, `0` is substituted for them.
    /// If the `local=true` parameter is set, the date/time is in local format; otherwise, it is in UTC format (default).
    CnDateTime(uint16_t year, uint16_t month, uint16_t day,
               uint16_t hour = 0,
               uint16_t minute = 0,
               uint16_t second = 0,
               uint16_t milliseconds = 0,
               bool local = false);

    /// \details Constructor creates a date and time object from the timestamp `CnTimestamp`.
    /// If the `local=true` parameter is set, the date/time is in local format; otherwise, it is in UTC format (default).
    CnDateTime(const CnTimestamp &t, bool local = false);

    /// \details Class destructor.
    ~CnDateTime();

    public:
    /// \details Returns the year of this date and time.
    uint16_t year() const;

    /// \details Returns the month of this date and time.
    uint16_t month() const;

    /// \details Returns the day of this date and time.
    uint16_t day() const;

    /// \details Returns the hour of this date and time.
    uint16_t hour() const;

    /// \details Returns the minute of this date and time.
    uint16_t minute() const;

    /// \details Returns the second of this date and time.
    uint16_t second() const;

    /// \details Returns the number of milliseconds of this date and time.
    uint16_t milliseconds() const;

    /// \details Returns the day of the week of this date and time.
    uint16_t dayOfWeek() const;

public:
    /// \details If this date and time is empty, the function returns `true`. Otherwise, it returns `false`.
    bool isNull() const;

    /// \details If this date and time is in local format, the function returns `true`. Otherwise, it returns `false`.
    bool isLocal() const;

    /// \details If this date and time is in UTC format, the function returns `true`. Otherwise, it returns `false`.
    bool isUtc() const;

    /// \details Creates a new `CnDatetime` object in local format based on this date and time.
    /// If this date and time is also in local format, the object is copied without any conversions.
    CnDateTime toLocal() const;

    /// \details Creates a new `CnDatetime` object in UTC format based on this date and time.
    /// If this date and time is also in UTC format, the object is copied without any conversions.
    CnDateTime toUtc() const;

    /// \details Returns a `CnTimestamp` timestamp created based on this date and time.
    CnTimestamp toTimestamp() const;

    /// \details Returns a `CnString` string in the format specified by the `format` parameter, created based on this date and time.
    /// The `format` parameter contains special specifiers that are replaced with the corresponding values; other elements of the string are output unchanged.\n
    /// Format specifiers:
    /// \li `%%Y` - replaced with the year value of this date/time. Output in 4-digit format
    /// \li `%%M` - replaced with the month value of this date/time. Output in 2-digit format
    /// \li `%%D` - replaced with the day value of this date/time. Output in 2-digit format
    /// \li `%%h` - replaced with the hour value of this date/time. Output in 2-digit format
    /// \li `%%m` - replaced with the minute value of this date/time. Output in 2-digit format
    /// \li `%%s` - replaced with the second value of this date/time. Output in 2-digit format
    /// \li `%%f` - replaced with the millisecond value of this date/time. Output in 3-digit format
    CnString toString(const CnString &format) const;

    /// \details Returns a `CnString` string in the format `%D.%M.%Y %h:%m:%s`, created based on this date and time.
    inline CnString toString() const { return toString(CnSTR("%D.%M.%Y %h:%m:%s.%f"));}

public:
    /// \details Clears this date and time and makes it empty (`null`).
    void clear();

    /// \details Clears the time of this date and time. After using it, the functions `hour()`, `minute()`, `second()`, `milliseconds()` will return `0`.
    void clearTimeOnly();

    /// \details Adds milliseconds to this date and time. If the `milliseconds` parameter is negative, milliseconds are subtracted.
    void addMilliseconds(int64_t milliseconds);

    /// \details Adds seconds to this date and time. If the `seconds` parameter is negative, seconds are subtracted.
    inline void addSeconds(int64_t seconds) { addMilliseconds(seconds * CN_MILLISEC_IN_SEC); }

    /// \details Adds minutes to this date and time. If the `minutes` parameter is negative, minutes are subtracted.
    inline void addMinutes(int64_t minutes) { addMilliseconds(minutes * CN_MILLISEC_IN_MINUTE); }

    /// \details Adds hours to this date and time. If the `hours` parameter is negative, hours are subtracted.
    inline void addHours(int64_t hours) { addMilliseconds(hours * CN_MILLISEC_IN_HOUR); }

    /// \details Adds days to this date and time. If the `days` parameter is negative, days are subtracted.
    inline void addDays(int64_t days) { addMilliseconds(days * CN_MILLISEC_IN_DAY); }

    /// \details 
    void swap(CnDateTime &other);

public:
    /// \details Assignment operator for `CnDateTime`.
    CnDateTime& operator=(const CnDateTime &other);

    /// \details Move operator for `CnDateTime`.
    CnDateTime& operator=(CnDateTime &&other);

    /// \details Equality comparison operator for `CnDateTime`. Returns `true` if date and time `l` is equivalent to `r`, otherwise `false`.
    inline friend bool operator==(const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() == r.toTimestamp(); }

    /// \details Inequality comparison operator for `CnDateTime`. Returns `true` if date and time `l` is not equivalent to `r`, otherwise `false`.
    inline friend bool operator!=(const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() != r.toTimestamp(); }

    /// \details Less-than comparison operator for `CnDateTime`. Returns `true` if date and time `l` is less (earlier) than `r`, otherwise `false`.
    inline friend bool operator< (const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() <  r.toTimestamp(); }

    /// \details Greater-than comparison operator for `CnDateTime`. Returns `true` if date and time `l` is greater (later) than `r`, otherwise `false`.
    inline friend bool operator> (const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() >  r.toTimestamp(); }

    /// \details Less-than-or-equal comparison operator for `CnDateTime`. Returns `true` if date and time `l` is not greater (not later) than `r`, otherwise `false`.
    inline friend bool operator<=(const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() <= r.toTimestamp(); }

    /// \details Greater-than-or-equal comparison operator for `CnDateTime`. Returns `true` if date and time `l` is not less (not earlier) than `r`, otherwise `false`.
    inline friend bool operator>=(const CnDateTime& l, const CnDateTime& r) { return l.toTimestamp() >= r.toTimestamp(); }

private:
    CnDateTimePrivate *d;
};

#endif // CNDATETIME_H