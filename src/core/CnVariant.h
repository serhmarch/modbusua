/*!
 * \file   CnVariant.h
 * \brief  Contains the description of the CnVariant class
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNVARIANT_H
#define CNVARIANT_H

#include "CnDateTime.h"

/*! \brief The `CnVariant` class is intended for working with values of different data types.

    \details The `CnVariant` class is designed to work with values of various data types.
    It can store values of different primitive types, such as integers, floating-point numbers,
    strings, timestamps, and date/time.

    The `CnVariant` class provides methods for converting between different types,
    checking the type of the stored value, and retrieving the value in the desired format.
    It also supports automatic type conversion when assigning values.

    \sa `CnDateTime`, `CnTimestamp`, `CnString`
*/
class CnVariant
{
public:
    /// \brief Enumeration of supported data types that can be stored within CnVariant.
    enum Type
    {
        vNull        , ///< Represents a null or uninitialized value.
        vBool        , ///< Boolean value (true or false).
        vInt8        , ///< Signed int 8 bit value.
        vUInt8       , ///< Unsigned int 8 bit value.
        vInt16       , ///< Signed int 16 bit value.
        vUInt16      , ///< Unsigned int 16 bit value.
        vInt32       , ///< Signed int 32 bit value.
        vInt=vInt32  , ///< Signed integer.
        vUInt32      , ///< Unsigned int 32 bit value.
        vUInt=vUInt32, ///< Unsigned integer.
        vInt64       , ///< Signed int 64 bit value.
        vUInt64      , ///< Unsigned int 64 bit value.
        vFloat       , ///< Floating-point value (single precision).
        vDouble      , ///< Floating-point value (double precision).
        vString      , ///< String value.
        vTimestamp   , ///< Timestamp value.
        vDateTime    , ///< Date and time value.
    };

public:
    /// \details The default constructor creates a `null` variant.
    CnVariant();

    /// \details Constructor `CnVariant` from a boolean value.
    CnVariant(bool v);

    /// \details Constructor `CnVariant` from a character value.
    CnVariant(int8_t v);

    /// \details Constructor `CnVariant` from an unsigned character value.
    CnVariant(uint8_t v);

    /// \details Constructor `CnVariant` from a short value.
    CnVariant(int16_t v);

    /// \details Constructor `CnVariant` from an unsigned short value.
    CnVariant(uint16_t v);

    /// \details Constructors `CnVariant` from long value.
    CnVariant(int32_t v);

    /// \details Constructors `CnVariant` from unsigned long value.
    CnVariant(uint32_t v);

    /// \details Constructors `CnVariant` from long long value.
    CnVariant(int64_t v);

    /// \details Constructors `CnVariant` from unsigned long long value.
    CnVariant(uint64_t v);

    /// \details Constructor `CnVariant` from a floating-point value.
    CnVariant(float v);

    /// \details Constructor `CnVariant` from a double-precision floating-point value.
    CnVariant(double v);

    /// \details Constructor `CnVariant` from a string value.
    CnVariant(const CnString &v);

    /// \details Constructor `CnVariant` from a null-terminated character array.
    CnVariant(const CnChar *v);

    /// \details Constructor `CnVariant` from a timestamp value.
    CnVariant(CnTimestamp v);

    /// \details Constructor `CnVariant` from a date and time value.
    CnVariant(const CnDateTime &v);

    /// \details Copy constructor.
    CnVariant(const CnVariant &other);

    /// \details Move constructor.
    CnVariant(CnVariant &&other);

    /// \details Destructor.
    ~CnVariant();

public:
    /// \details Returns the type of the stored value.
    inline Type type() const { return m_type; }

    /// \details Returns `true` if the variant is null, `false` otherwise.
    inline bool isNull() const { return m_type == vNull; }

    /// \details Returns a pointer to the internal data of the variant.
    /// This can be used for low-level access or serialization.
    inline const void* data() const { return &m_int64; }

    /// \details Returns a pointer to the internal data of the variant.
    /// This can be used for low-level access or serialization.
    inline void* data() { return &m_int64; }

    /// \details Clears the current value and sets the variant to `null`.
    void clear();

    /// \details Converts the variant to a boolean value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an bool; otherwise *ok is set to false.
    bool toBool(bool *ok = nullptr) const;

    /// \details Converts the variant to a character value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a char; otherwise *ok is set to false.
    int8_t toInt8(bool *ok = nullptr) const;

    /// \details Converts the variant to an unsigned character value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an unsigned char; otherwise *ok is set to false.
    uint8_t toUInt8(bool *ok = nullptr) const;

    /// \details Converts the variant to a short value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a short; otherwise *ok is set to false.
    int16_t toInt16(bool *ok = nullptr) const;

    /// \details Converts the variant to an unsigned short value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an unsigned short; otherwise *ok is set to false.
    uint16_t toUInt16(bool *ok = nullptr) const;

    /// \details Converts the variant to a 32-bit integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a 32-bit integer; otherwise *ok is set to false.
    int32_t toInt32(bool *ok = nullptr) const;

    /// \details Converts the variant to an unsigned 32-bit integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an unsigned 32-bit integer; otherwise *ok is set to false.
    uint32_t toUInt32(bool *ok = nullptr) const;

    /// \details Converts the variant to a 64-bit integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a 64-bit integer; otherwise *ok is set to false.
    int64_t toInt64(bool *ok = nullptr) const;

    /// \details Converts the variant to an unsigned 64-bit integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an unsigned 64-bit integer; otherwise *ok is set to false.
    uint64_t toUInt64(bool *ok = nullptr) const;

    /// \details Converts the variant to a floating-point value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a float; otherwise *ok is set to false.
    float toFloat(bool *ok = nullptr) const;

    /// \details Converts the variant to a double-precision floating-point value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a double; otherwise *ok is set to false.
    double toDouble(bool *ok = nullptr) const;

    /// \details Converts the variant to a string value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a string; otherwise *ok is set to false.
    CnString toString(bool *ok = nullptr) const;

    /// \details Converts the variant to a timestamp value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a timestamp; otherwise *ok is set to false.
    CnTimestamp toTimestamp(bool *ok = nullptr) const;

    /// \details Converts the variant to a date and time value.
    /// If ok is non-null: *ok is set to true if the value could be converted to a date and time; otherwise *ok is set to false.
    CnDateTime toDateTime(bool *ok = nullptr) const;

    /// \details Converts the variant to an integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an int; otherwise *ok is set to false.
    inline int toInt(bool *ok = nullptr) const { return static_cast<int>(toInt32(ok)); }

    /// \details Converts the variant to an unsigned integer value.
    /// If ok is non-null: *ok is set to true if the value could be converted to an unsigned int; otherwise *ok is set to false.
    inline unsigned int toUInt(bool *ok = nullptr) const { return static_cast<unsigned int>(toUInt32(ok)); }

public:
    /// \details Copy assignment operator.
    /// \sa `CnVariant(const CnVariant &other)`
    CnVariant& operator=(const CnVariant &other);

    /// \details Move assignment operator.
    /// \sa `CnVariant(CnVariant &&other)`
    CnVariant& operator=(CnVariant &&other);

private:
    Type m_type;
    union 
    {
        bool m_bool;
        int8_t m_int8;
        uint8_t m_uint8;
        int16_t m_int16;
        uint16_t m_uint16;
        int32_t m_int32;
        uint32_t m_uint32;
        int64_t m_int64;
        uint64_t m_uint64;
        float m_float;
        double m_double;
        CnString *m_string;
        CnTimestamp m_timestamp;
        CnDateTime *m_datetime;
    };
};

#endif // CNVARIANT_H