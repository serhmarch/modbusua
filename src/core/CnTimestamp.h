/*! 
 * \file   CnTimestamp.h
 * \brief  Contains the description of the timestamp class
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNTIMESTAMP_H
#define CNTIMESTAMP_H

#include "CnCore.h"

/*! \brief The `CnTimestamp` class is designed for convenient work with timestamps.

    \details The `CnTimestamp` class is used for working with timestamps.
    `CnTimestamp` constains a 64-bit unsigned integer value representing
    the number of 100-nanosecond intervals since January 1, 1601 (UTC).

    The `CnTimestamp` class always(!) stores its value **in UTC format**.
    However, it has functions for converting the timestamp value to local format.\n
    To "unpack" the timestamp value into separate variables such as day, month, hour, etc.,
    you should use the `CnDateTime` class.

    \sa `CnDateTime`
*/

class CnTimestamp
{
public:
    /// \details Returns the current timestamp.
    static CnTimestamp current();

    /// \details Returns the current high-precision timestamp.
    /// For WinAPI function `GetSystemTimePreciseAsFileTime` is used.
    static CnTimestamp currentPrecise();

    /// \details Returns a timestamp for the UNIX UTC format in milliseconds.
    static CnTimestamp fromUtcMSecSinceEpoch(int64_t msec);

    /// \details Returns a timestamp for the raw value (number of 100-nanosecond intervals since January 1, 1601 (UTC)).
    inline static CnTimestamp fromRawValue(uint64_t nanosec100) { CnTimestamp t; t.m_data = nanosec100; return t; }

public:
    /// \details The default constructor creates an empty (`null`) timestamp.
    CnTimestamp();

    /// \details Copy constructor. Creates a copy of the `other` object.
    CnTimestamp(const CnTimestamp& other);

    /// \details Constructor creates a timestamp object from the raw value.
    /// If the `local=true` parameter is set, it is assumed that the `rawValue` parameter is in local format;
    /// otherwise, it is in UTC format (default).
    CnTimestamp(uint64_t rawValue, bool local = false);

public:
    /// \details Returns the "raw" value of the timestamp.
    /// The format used is a 64-bit unsigned value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
    inline uint64_t rawValue() const { return m_data; }

    /// \details Same as `rawValue()`.
    /// \sa `rawValue()`
    inline uint64_t data() const { return rawValue(); }

    /// \details Adds `raw` 100-nanoseconds to this timestamp. If the `raw` parameter is negative, 100-nanoseconds are subtracted.
    inline void addRawValue(int64_t raw) { m_data += raw; }

    /// \details Converts this timestamp to a `uint64_t` value in UTC format.
    inline uint64_t toUtcRawValue() const { return rawValue(); }

    /// \details Converts this timestamp to a `uint64_t` value in local format.
    uint64_t toLocalRawValue() const;

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of milliseconds since January 1, 1970 (Unix epoch).
    uint64_t toUtcMSecSinceEpoch() const;

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of seconds since January 1, 1970 (Unix epoch).
    inline uint64_t toUtcSecSinceEpoch() const { return toUtcMSecSinceEpoch() / CN_MILLISEC_IN_SEC; }

    /// \details Returns a 64-bit unsigned value in local format representing the number of milliseconds since January 1, 1970 (Unix epoch).
    uint64_t toLocalMSecSinceEpoch() const;

    /// \details Returns a 64-bit unsigned value in local format representing the number of seconds since January 1, 1970 (Unix epoch).
    inline uint64_t toLocalSecSinceEpoch() const { return toLocalMSecSinceEpoch() / CN_MILLISEC_IN_SEC; }

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of milliseconds since January 1, 1970 (Unix epoch).
    /// Same as `toUtcMSecSinceEpoch()`.
    inline uint64_t toMSecSinceEpoch() const { return toUtcMSecSinceEpoch(); }

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of seconds since January 1, 1970 (Unix epoch).
    /// Same as `toUtcSecSinceEpoch()`.
    inline uint64_t toSecSinceEpoch() const { return toUtcSecSinceEpoch(); }

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of milliseconds since January 1, 1980.
    uint64_t toUtcMSec1980() const;

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of seconds since January 1, 1980.
    inline uint64_t toUtcSec1980() const { return toUtcMSec1980() / CN_MILLISEC_IN_SEC; }

    /// \details Returns a 64-bit unsigned value in local format representing the number of milliseconds since January 1, 1980.
    uint64_t toLocalMSec1980() const;

    /// \details Returns a 64-bit unsigned value in local format representing the number of seconds since January 1, 1980.
    inline uint64_t toLocalSec1980() const { return toLocalMSec1980() / CN_MILLISEC_IN_SEC; }

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of milliseconds since January 1, 1980.
    /// Same as `toUtcMSec1980()`.
    inline uint64_t toMSec1980() const { return toUtcSec1980(); }

    /// \details Returns a 64-bit unsigned value in UTC format representing the number of seconds since January 1, 1980.
    /// Same as `toUtcSec1980()`.
    inline uint64_t toSec1980() const { return toUtcSec1980(); }

public:
    /// \details Returns a 64-bit unsigned value representing the number of seconds since January 1, 1601 (UTC).
    inline uint64_t seconds() const { return m_data / CN_NANOSEC100_IN_SEC; }

    /// \details Adds seconds to this timestamp. If the `seconds` parameter is negative, seconds are subtracted.
    inline void addSeconds(int64_t sec) { m_data += sec * CN_NANOSEC100_IN_SEC; }

    /// \details Returns a 64-bit unsigned value representing the number of milliseconds since January 1, 1601 (UTC).
    inline uint64_t milliseconds() const { return m_data / CN_NANOSEC100_IN_MILLISEC; }

    /// \details Adds milliseconds to this timestamp. If the `millis` parameter is negative, milliseconds are subtracted.
    inline void addMilliseconds(int64_t millis) { m_data += millis * CN_NANOSEC100_IN_MILLISEC; }

    /// \details Returns a 64-bit unsigned value representing the number of microseconds since January 1, 1601 (UTC).
    inline uint64_t microseconds() const { return m_data / CN_NANOSEC100_IN_MICROSEC; }

    /// \details Adds microseconds to this timestamp. If the `micros` parameter is negative, microseconds are subtracted.
    inline void addMicroseconds(int64_t micros) { m_data += micros * CN_NANOSEC100_IN_MICROSEC; }

public:
    /// \details Conversion operator to a 64-bit unsigned value representing `rawValue()`.
    /// \sa `rawValue()`
    inline operator uint64_t() const { return rawValue(); }

    /// \details Assignment operator to set the timestamp `other` to the current timestamp.
    inline CnTimestamp &operator=(const CnTimestamp &other) { m_data = other.m_data; return *this; }

    /// \details Assignment operator for a 64-bit unsigned value representing the number of milliseconds since January 1, 1601 (UTC).
    inline CnTimestamp &operator=(uint64_t rawValue) { m_data = rawValue; return *this; }

    /// \details Addition operator for a 64-bit unsigned value representing the number of milliseconds to the current timestamp.
    inline CnTimestamp &operator+=(uint64_t rawValue) { m_data += rawValue; return *this; }

    /// \details Subtraction operator for a 64-bit unsigned value representing the number of milliseconds from the current timestamp.
    inline CnTimestamp &operator-=(uint64_t rawValue) { m_data -= rawValue; return *this; }

    /// \details Equality comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is equivalent to `r`, otherwise `false`.
    inline friend bool operator==(const CnTimestamp& l, const CnTimestamp& r) { return l.m_data == r.m_data; }

    /// \details Inequality comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is not equivalent to `r`, otherwise `false`.
    inline friend bool operator!=(const CnTimestamp& l, const CnTimestamp& r) { return l.m_data != r.m_data; }

    /// \details Less-than comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is less (earlier) than `r`, otherwise `false`.
    inline friend bool operator< (const CnTimestamp& l, const CnTimestamp& r) { return l.m_data <  r.m_data; }

    /// \details Greater-than comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is greater (later) than `r`, otherwise `false`.
    inline friend bool operator> (const CnTimestamp& l, const CnTimestamp& r) { return l.m_data >  r.m_data; }

    /// \details Less-than-or-equal comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is not greater (not later) than `r`, otherwise `false`.
    inline friend bool operator<=(const CnTimestamp& l, const CnTimestamp& r) { return l.m_data <= r.m_data; }

    /// \details Greater-than-or-equal comparison operator for `CnTimestamp`. Returns `true` if timestamp `l` is not less (not earlier) than `r`, otherwise `false`.
    inline friend bool operator>=(const CnTimestamp& l, const CnTimestamp& r) { return l.m_data >= r.m_data; }

private:
    uint64_t m_data; //
};

#endif // CNTIMESTAMP_H