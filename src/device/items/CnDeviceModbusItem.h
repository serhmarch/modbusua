/*! 
 * \file   CnDeviceModbusItem.h
 * \brief  Defines Modbus `item reference` list for a device/PLC
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDEVICEMODBUSITEM_H
#define CNDEVICEMODBUSITEM_H

#include <CnSharedPointer.h>
#include "CnDeviceBaseItem.h"
#include <device/CnDevice.h>

class CnDeviceMessage;

// ------------------------------------------------------------------------------------------
// ----------------------------------- CnDeviceModbusItem -----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Base Modbus `item reference` class for a device/PLC.

    \details This server (driver) allows direct work with Modbus registers similar to the standard MBTCP ASP driver.
    The item reference format is:\n
    `{0|1|3|4}xxxxx[-{0|1|3|4}yyyyy <arraysuffix>|<suffix>][, msgid=<id>]`\n
    `{0|1|3|4}<xxxxx>` – Modbus register number, `[000001-065536]`, `[100001-165536]`, `[300001-365536]`, `[400001-465536]`\n
    `{0|1|3|4}<yyyyy>` – end Modbus register number for array reads, `[000001-065536]`, `[100001-165536]`,
    `[300001-365536]`, `[400001-465536]`. An array is read starting from `{0|1|3|4}<xxxxx>` up to and including `{0|1|3|4}<yyyyy>`;\n
    `<arraysuffix>` – data-type suffix for reading an array of registers (mandatory for register arrays). Possible values:\n
    \li `B` – array has type `String`, representing a sequence of bytes, each represented by two hexadecimal (HEX) characters
    (no spaces or separators). Characters A–F must be uppercase. High bits of a byte go first (left), low bits last (right).
    The low byte of a register goes first (left), the high byte second (right). For the following PLC memory snapshot:
    400001|400002|...|400010
    ------|------|---|------
    0x0BCD|0x80FE|...|0xFC05
    The resulting item reference is: `400001-400010 B = "CD0BFE80...05FC"`
    
    `<suffix>` – data-type suffix for the item reference (optional). Described below. By default (if suffix is omitted),
    data type is taken as UInt16 for 3x and 4x registers, and Bool for 0x and 1x.
    `msgid=<id>` – Modbus message identifier (optional). Explicitly groups Modbus registers into Modbus messages. `<id>` can be a number or a word.
 */

class CnDeviceModbusItem : public CnDeviceBaseItem
{
public:
    /// \details Class constructor.
    /// \param[in]  szItemName Variable name (set in configurator field 'Item Reference')
    /// \param[in]  device     Pointer to an object of type `CnDevice` that stores all device/PLC runtime parameters
    /// \param[in]  offset     Offset from the beginning of the Modbus memory area for the variable
    /// \param[in]  messageId  Modbus message identifier set in `'Item Reference'` as `msgid=<id>`. Explicitly groups several variables into one Modbus message
    CnDeviceModbusItem(CnDevice *device, uint16_t offset, int period, const CnString &messageId = CnString());

public:
    /// \details 
    inline void lock() { m_cs.lock(); }
    
    /// \details 
    inline void unlock() { m_cs.unlock(); }
    
    /// \details Returns the `CnDeviceBaseItem::ItemType` of this ItemReference as `CnDeviceBaseItem::Item_Modbus`.
    CnDeviceBaseItem::ItemType itemType() const override { return CnDeviceBaseItem::Item_Modbus; }

    /// \details 
    virtual Cn::DataSuffix dataSuffix() const { return Cn::DataTypeToSuffix(dataType()); }

    /// \details Returns the Modbus message identifier set in `'Item Reference'` as `msgid=<id>`.
    inline const CnString &messageId() const { return m_messageId; }

    /// \details Returns the address (offset counted from 0) of the corresponding memory cell of the remote device.
    inline uint16_t offset() const { return m_offset; }

    /// \details Returns the memory type (`0x|1x|3x|4x`) of the corresponding memory cell of the remote device.
    virtual Modbus::MemoryType memoryType() const = 0;

    /// \details Returns the number of registers/bits of the corresponding memory cell of the remote device,
    /// i.e., sets the variable size in registers (for 3x or 4x) or bits (for 0x or 1x).
    virtual uint16_t count() const = 0;

    /// \details Returns the memory pointer for the corresponding memory cell of the remote device.
    virtual void *data() = 0;

    /// \details Returns the polling period in milliseconds.
    inline int period() const { return m_period; }

    /// \details Sets the polling period in milliseconds.
    inline void setPeriod(int period) { m_period = period; }

    /// \details Returns `true` if this element is not available for polling.
    inline bool isReadDisabled() const { return m_period == CN_MODBUS_PERIOD_DISABLE_VALUE; }

    /// \details Returns a pointer to the Modbus message object this item belongs to.
    inline CnDeviceMessage *message() const { CnCriticalSectionLocker _(&m_cs); return m_message; }

    /// \details Sets a pointer to the Modbus message object this item belongs to.
    void setMessage(CnDeviceMessage *message);

    /// \details
    inline CnTimestamp timestampLastRequest() const { CnCriticalSectionLocker _(&m_cs); return m_timestampLastRequest; }

    /// \details
    inline void refreshTimestampLastRequest() { CnCriticalSectionLocker _(&m_cs); m_timestampLastRequest = CnTimestamp::current(); }

    /// \details
    virtual void processDataUnsafe();

    /// \details Sets only the status value and timestamp.
    inline void updateStatusTimestampUnsafe(Cn::StatusCode status, CnTimestamp timestamp) 
    { 
        m_status = status;
        m_timestamp = timestamp; 
    }

    /// \details
    void update(const CnVariant &value, Cn::StatusCode status = Cn::Status_Good, CnTimestamp timestamp = CnTimestamp()) override;

protected:
    virtual void updateInner(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) = 0;
    void resetInner();
    inline Cn::StatusCode statusInner() const
    {
        if (m_device->isDataAvailable())
            return m_status;
        return Cn::Status_BadNotConnected;
    }
    
protected:
    mutable CnCriticalSection m_cs;
    uint16_t m_offset;
    int m_period;
    CnString m_messageId;
    CnDeviceMessage *m_message;
    Cn::StatusCode m_status;
    CnTimestamp m_timestamp;
    CnTimestamp m_timestampLastRequest;
};


// ------------------------------------------------------------------------------------------
// ----------------------------------- CnDeviceModbusItemT ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class for a device/PLC.

    \details Generates common code for all Modbus variables.
    Reading is implemented in `readFromMessage()`, writing in `writeToMessage()`.
 */

template <class T>
class CnDeviceModbusItemT : public CnDeviceModbusItem
{
public:
    CnDeviceModbusItemT(CnDevice *device, uint16_t offset, int period, const CnString &messageId = CnString()) :
        CnDeviceModbusItem(device, offset, period, messageId),
        m_value(T()) {}

public:
    /// \details Returns the corresponding `Cn::DataType` for the Modbus variable.
    Cn::DataType dataType() const override { return Cn::toDataType<T>(); }

    void *data() override { return &this->m_value; }

    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        CnCriticalSectionLocker _(&this->m_cs);        
        if (status)
            *status = this->statusInner();
        if (timestamp)
            *timestamp = this->m_timestamp;
        return CnVariant(this->m_value);
    }

public:
    /// \details Returns number of registers of the corresponding memory cell, i.e., variable size in registers.
    virtual uint16_t count() const { return sizeof(T) / MB_REGE_SZ_BYTES; }

protected:
    void updateInner(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) override
    {
        this->m_status = status;
        this->m_timestamp = timestamp;
        if (!value.isNull())
            Cn::fromVariant(value, &this->m_value);
    }

protected:
    T m_value;
};

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDeviceModbusItemSwappedT -------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template `item reference` class for Modbus register-type variables with swapped register order.

    \details Derived from `CnDeviceModbusItemT`, with the same properties,
    differing only in the order of 16-bit registers when representing the value.
 */

template <class T>
class CnDeviceModbusItemSwappedT : public CnDeviceModbusItemT<T>
{
public:
    using CnDeviceModbusItemT<T>::CnDeviceModbusItemT;

public:
    /// \details 
    Cn::DataSuffix dataSuffix() const override { return Cn::DataTypeToSwappedSuffix(this->dataType()); }

protected:
    void processDataUnsafe() override
    {
        this->m_value = Cn::swapped(this->m_value);
    }

    void updateInner(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) override
    {
        this->m_status = status;
        this->m_timestamp = timestamp;
        if (!value.isNull())
        {
            T v;
            Cn::fromVariant(value, &v);
            v = Cn::swapped(v);
            this->m_value = v;
        }
    }
};

// ------------------------------------------------------------------------------------------
// --------------------------------- CnDeviceModbusItemBitsT --------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of bit type for a device/PLC.

    \details Generates common code for all bit-type Modbus variables.
    Reading is implemented in `readFromMessage()`, writing in `writeToMessage()`.
 */

template <class T>
class CnDeviceModbusItemBitsT : public CnDeviceModbusItemT<T>
{
public:
    using CnDeviceModbusItemT<T>::CnDeviceModbusItemT;

public:
    /// \details Returns number of bits of the corresponding memory cell,
    /// i.e., variable size in bits.
    virtual uint16_t count() const override { return sizeof(T) * MB_BYTE_SZ_BITES; }
};


// ------------------------------------------------------------------------------------------
// ----------------------------- CnDeviceModbusItemSwappedBitsT -----------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template `item reference` class for Modbus bit-type variables with swapped register order.

    \details Derived from `CnDeviceModbusItemT`, with the same properties,
    differing only in the order of 16-bit registers when representing the value.
 */

template <class T>
class CnDeviceModbusItemSwappedBitsT : public CnDeviceModbusItemSwappedT<T>
{
public:
    using CnDeviceModbusItemSwappedT<T>::CnDeviceModbusItemSwappedT;

public:
    /// \details Returns number of bits of the corresponding memory cell,
    /// i.e., variable size in bits.
    virtual uint16_t count() const override { return sizeof(T) * MB_BYTE_SZ_BITES; }
};


// ------------------------------------------------------------------------------------------
// --------------------------------- CnDeviceModbusItemRegsT --------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of register type for a device/PLC.

    \details Generates common code for all register-type Modbus variables.
    Reading is implemented in `readFromMessage()`, writing in `writeToMessage()`.
 */

template <class T>
class CnDeviceModbusItemRegsT : public CnDeviceModbusItemT<T>
{
public:
    using CnDeviceModbusItemT<T>::CnDeviceModbusItemT;

public:
    /// \details Returns number of registers of the corresponding memory cell,
    /// i.e., variable size in registers.
    virtual uint16_t count() const override { return sizeof(T) / MB_REGE_SZ_BYTES; }
};


// ------------------------------------------------------------------------------------------
// ----------------------------- CnDeviceModbusItemSwappedRegsT -----------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template `item reference` class for Modbus register-type variables with swapped register order.

    \details Derived from `CnDeviceModbusItemT`, with the same properties,
    differing only in the order of 16-bit registers when representing the value.
 */

template <class T>
class CnDeviceModbusItemSwappedRegsT : public CnDeviceModbusItemSwappedT<T>
{
public:
    using CnDeviceModbusItemSwappedT<T>::CnDeviceModbusItemSwappedT;

public:
    /// \details Returns number of registers of the corresponding memory cell,
    /// i.e., variable size in registers.
    virtual uint16_t count() const override { return sizeof(T) / MB_REGE_SZ_BYTES; }
};


// ------------------------------------------------------------------------------------------
// ------------------------------- CnDeviceModbusItemByteArray ------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Base `item reference` class for Modbus byte-array variables.

    \details The array has type String, representing a sequence of bytes,
    each represented by two hexadecimal (HEX) characters (no spaces or separators).
    Characters A–F must be uppercase. High bits of a byte go first (left), low bits last (right).
    The low byte of a register goes first (left), the high byte second (right).

    \sa CnDeviceModbusItem
 */

class CnDeviceModbusItemByteArray : public CnDeviceModbusItem
{
public:
    using CnDeviceModbusItem::CnDeviceModbusItem;

public:
    /// \details Returns `Cn::Data_String` as the `Cn::DataType` for the byte-array variable.
    Cn::DataType dataType() const override { return Cn::Data_String; }
    void *data() override { return m_data.data(); }
    Cn::DataSuffix dataSuffix() const override { return Cn::Suffix_ByteArray; }
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override;
    void updateInner(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) override;

protected:
    CnString toString() const;
    bool fillByteArray(const CnString &s, uint8_t *buff, uint16_t szBuff) const;
    inline bool fillByteArray(const CnString &s, CnByteArray &data) const { return fillByteArray(s, reinterpret_cast<uint8_t*>(data.data()), static_cast<uint16_t>(data.size())); }

protected:
    CnByteArray m_data;
};


/*! \brief Base `item reference` class for Modbus byte-array variables for bit-type Modbus messages.

    \details The array has type String, representing a sequence of bytes,
    each represented by two hexadecimal (HEX) characters (no spaces or separators).
    Characters A–F must be uppercase. High bits of a byte go first (left), low bits last (right).
    The low byte of a register goes first (left), the high byte second (right).

    \sa CnDeviceModbusItemByteArray
 */

class CnDeviceModbusItemBitsByteArray : public CnDeviceModbusItemByteArray
{
public:
    /// \details Class constructor.
    /// \param[in]  szItemName Variable name (set in configurator field 'Item Reference')
    /// \param[in]  device     Pointer to an object of type `CnDevice` that stores all device/PLC runtime parameters
    /// \param[in]  offset     Offset from the beginning of the Modbus byte-array memory area
    /// \param[in]  count      Size of the Modbus byte-array in bits
    /// \param[in]  messageId  Modbus message identifier set in `'Item Reference'` as `msgid=<id>`. Explicitly groups several variables into one Modbus message
    CnDeviceModbusItemBitsByteArray(CnDevice *device, uint16_t offset, uint16_t count, int period, const CnString &messageId = CnString());

public:
    /// \details Returns size of the Modbus byte-array in bits
    uint16_t count() const override { return m_count; }

protected:
    uint16_t m_count;
};

/*! \brief Base `item reference` class for Modbus byte-array variables for register-type Modbus messages.

    \details The array has type String, representing a sequence of bytes,
    each represented by two hexadecimal (HEX) characters (no spaces or separators).
    Characters A–F must be uppercase. High bits of a byte go first (left), low bits last (right).
    The low byte of a register goes first (left), the high byte second (right).

    \sa CnDeviceModbusItemByteArray
 */

class CnDeviceModbusItemRegsByteArray : public CnDeviceModbusItemByteArray
{
public:
    /// \details Class constructor.
    /// \param[in]  szItemName Variable name (set in configurator field 'Item Reference')
    /// \param[in]  device     Pointer to an object of type `CnDevice` that stores all device/PLC runtime parameters
    /// \param[in]  offset     Offset from the beginning of the Modbus byte-array memory area
    /// \param[in]  count      Size of the Modbus byte-array in registers
    /// \param[in]  messageId  Modbus message identifier set in `'Item Reference'` as `msgid=<id>`. Explicitly groups several variables into one Modbus message
    CnDeviceModbusItemRegsByteArray(CnDevice *device, uint16_t offset, uint16_t count, int period, const CnString &messageId = CnString());

public:
    /// \details Returns size of the Modbus byte-array in registers
    uint16_t count() const override { return static_cast<uint16_t>(m_data.size() / MB_REGE_SZ_BYTES); }
};

// ------------------------------------------------------------------------------------------
// ---------------------------------- CnDeviceModbusItem0x ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of type 'discrete output' (coil, 0x-bit) for a device/PLC.

    \details Generates common code for all 'discrete output' (coil, 0x-bit) Modbus variables.
 */

template <class T>
class CnDeviceModbusItem0xT : public CnDeviceModbusItemBitsT<T>
{
public:
    using CnDeviceModbusItemBitsT<T>::CnDeviceModbusItemBitsT;

public:
    /// \details  The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Returns memory type `0x` for a 'discrete output' (coil, 0x-bit) cell.
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_0x; }
};

/*! \brief Modbus `item reference` class of boolean type for 'discrete output' (coil, 0x-bit) memory cell.

    \details Generates common code for all 'discrete output' (coil, 0x-bit) Modbus variables.
 */

class CnDeviceModbusItem0xBool : public CnDeviceModbusItem0xT<bool>
{
public:
    using CnDeviceModbusItem0xT::CnDeviceModbusItem0xT;

public:
    /// \details Returns variable size in bits. For boolean always returns `1`.
    virtual uint16_t count() const override { return 1; }
};

typedef CnDeviceModbusItem0xT<int16_t > CnDeviceModbusItem0xInt16 ; //!< Modbus 'discrete output' (coil, 0x-bit) type `Int16`
typedef CnDeviceModbusItem0xT<uint16_t> CnDeviceModbusItem0xUInt16; //!< Modbus 'discrete output' (coil, 0x-bit) type `UInt16`
typedef CnDeviceModbusItem0xT<int32_t > CnDeviceModbusItem0xInt32 ; //!< Modbus 'discrete output' (coil, 0x-bit) type `Int32`
typedef CnDeviceModbusItem0xT<uint32_t> CnDeviceModbusItem0xUInt32; //!< Modbus 'discrete output' (coil, 0x-bit) type `UInt32`
typedef CnDeviceModbusItem0xT<int64_t > CnDeviceModbusItem0xInt64 ; //!< Modbus 'discrete output' (coil, 0x-bit) type `Int64`
typedef CnDeviceModbusItem0xT<uint64_t> CnDeviceModbusItem0xUInt64; //!< Modbus 'discrete output' (coil, 0x-bit) type `UInt64`
typedef CnDeviceModbusItem0xT<float   > CnDeviceModbusItem0xFloat ; //!< Modbus 'discrete output' (coil, 0x-bit) type `Float`
typedef CnDeviceModbusItem0xT<double  > CnDeviceModbusItem0xDouble; //!< Modbus 'discrete output' (coil, 0x-bit) type `Double`


/*! \brief Template `item reference` class for 'discrete output' (coil, 0x-bit) with swapped register order.

    \details Derived from `CnDeviceModbusItemSwappedBitsT`,
    specializing for 'discrete output' (coil, 0x-bit) values.
 */

template <class T>
class CnDeviceModbusItemSwapped0xT : public CnDeviceModbusItemSwappedBitsT<T>
{
public:
    using CnDeviceModbusItemSwappedBitsT<T>::CnDeviceModbusItemSwappedBitsT;

public:
    /// \details  The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Returns memory type `0x` for a 'discrete output' (coil, 0x-bit) cell.
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_0x; }
};

typedef CnDeviceModbusItemSwapped0xT<int32_t > CnDeviceModbusItem0xSwappedInt32 ; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `Int32`
typedef CnDeviceModbusItemSwapped0xT<uint32_t> CnDeviceModbusItem0xSwappedUInt32; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `UInt32`
typedef CnDeviceModbusItemSwapped0xT<int64_t > CnDeviceModbusItem0xSwappedInt64 ; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `Int64`
typedef CnDeviceModbusItemSwapped0xT<uint64_t> CnDeviceModbusItem0xSwappedUInt64; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `UInt64`
typedef CnDeviceModbusItemSwapped0xT<float   > CnDeviceModbusItem0xSwappedFloat ; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `Float`
typedef CnDeviceModbusItemSwapped0xT<double  > CnDeviceModbusItem0xSwappedDouble; //!< Modbus 'discrete output' (coil, 0x-bit) swapped-register type `Double`

/*! \brief Modbus `item reference` class of byte-array type for 'discrete output' (coil, 0x-bit) memory cell.

    \details Generates common code for all 'discrete output' (coil, 0x-bit) Modbus variables.
 */

class CnDeviceModbusItem0xByteArray : public CnDeviceModbusItemBitsByteArray
{
public:
    using CnDeviceModbusItemBitsByteArray::CnDeviceModbusItemBitsByteArray;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `0x` for 'discrete output' (coil, 0x-bit).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_0x; }
};


// ------------------------------------------------------------------------------------------
// ---------------------------------- CnDeviceModbusItem1x ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of type 'discrete input' (1x-bit) for a device/PLC.

    \details Generates common code for all 'discrete input' (1x-bit) Modbus variables.
 */

template <class T>
class CnDeviceModbusItem1xT : public CnDeviceModbusItemBitsT<T>
{
public:
    using CnDeviceModbusItemBitsT<T>::CnDeviceModbusItemBitsT;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `1x` for 'discrete input' (1x-bit).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_1x; }
};


/*! \brief Modbus `item reference` class of boolean type for 'discrete input' (1x-bit) memory cell.

    \details Generates common code for all 'discrete input' (1x-bit) Modbus variables.
 */

class CnDeviceModbusItem1xBool : public CnDeviceModbusItem1xT<bool>
{
public:
    using CnDeviceModbusItem1xT<bool>::CnDeviceModbusItem1xT;

public:
    /// \details Returns variable size in bits. For boolean always returns `1`.
    virtual uint16_t count() const override { return 1; }
};

typedef CnDeviceModbusItem1xT<int16_t > CnDeviceModbusItem1xInt16 ; //!< Modbus 'discrete input' (1x-bit) type `Int16`
typedef CnDeviceModbusItem1xT<uint16_t> CnDeviceModbusItem1xUInt16; //!< Modbus 'discrete input' (1x-bit) type `UInt16`
typedef CnDeviceModbusItem1xT<int32_t > CnDeviceModbusItem1xInt32 ; //!< Modbus 'discrete input' (1x-bit) type `Int32`
typedef CnDeviceModbusItem1xT<uint32_t> CnDeviceModbusItem1xUInt32; //!< Modbus 'discrete input' (1x-bit) type `UInt32`
typedef CnDeviceModbusItem1xT<int64_t > CnDeviceModbusItem1xInt64 ; //!< Modbus 'discrete input' (1x-bit) type `Int64`
typedef CnDeviceModbusItem1xT<uint64_t> CnDeviceModbusItem1xUInt64; //!< Modbus 'discrete input' (1x-bit) type `UInt64`
typedef CnDeviceModbusItem1xT<float   > CnDeviceModbusItem1xFloat ; //!< Modbus 'discrete input' (1x-bit) type `Float`
typedef CnDeviceModbusItem1xT<double  > CnDeviceModbusItem1xDouble; //!< Modbus 'discrete input' (1x-bit) type `Double`

/*! \brief Template `item reference` class for 'discrete input' (1x-bit) with swapped register order.

    \details Derived from `CnDeviceModbusItemSwappedBitsT`,
    specializing for 'discrete input' (1x-bit) values.
 */

template <class T>
class CnDeviceModbusItemSwapped1xT : public CnDeviceModbusItemSwappedBitsT<T>
{
public:
    using CnDeviceModbusItemSwappedBitsT<T>::CnDeviceModbusItemSwappedBitsT;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `1x` for 'discrete input' (1x-bit).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_1x; }
};


typedef CnDeviceModbusItemSwapped1xT<int32_t > CnDeviceModbusItem1xSwappedInt32 ; //!< Modbus 'discrete input' (1x-bit) swapped-register type `Int32`
typedef CnDeviceModbusItemSwapped1xT<uint32_t> CnDeviceModbusItem1xSwappedUInt32; //!< Modbus 'discrete input' (1x-bit) swapped-register type `UInt32`
typedef CnDeviceModbusItemSwapped1xT<int64_t > CnDeviceModbusItem1xSwappedInt64 ; //!< Modbus 'discrete input' (1x-bit) swapped-register type `Int64`
typedef CnDeviceModbusItemSwapped1xT<uint64_t> CnDeviceModbusItem1xSwappedUInt64; //!< Modbus 'discrete input' (1x-bit) swapped-register type `UInt64`
typedef CnDeviceModbusItemSwapped1xT<float   > CnDeviceModbusItem1xSwappedFloat ; //!< Modbus 'discrete input' (1x-bit) swapped-register type `Float`
typedef CnDeviceModbusItemSwapped1xT<double  > CnDeviceModbusItem1xSwappedDouble; //!< Modbus 'discrete input' (1x-bit) swapped-register type `Double`


/*! \brief Modbus `item reference` class of byte-array type for 'discrete input' (1x-bit) memory cell.

    \details Generates common code for all 'discrete input' (1x-bit) Modbus variables.
 */

class CnDeviceModbusItem1xByteArray : public CnDeviceModbusItemBitsByteArray
{
public:
    using CnDeviceModbusItemBitsByteArray::CnDeviceModbusItemBitsByteArray;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `1x` for 'discrete input' (1x-bit).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_1x; }
};


// ------------------------------------------------------------------------------------------
// ---------------------------------- CnDeviceModbusItem3x ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of type 'analog input' (3x-register) for a device/PLC.

    \details Generates common code for all 'analog input' (3x-register) Modbus variables.
 */

template <class T>
class CnDeviceModbusItem3xT : public CnDeviceModbusItemRegsT<T>
{
public:
    using CnDeviceModbusItemRegsT<T>::CnDeviceModbusItemRegsT;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `3x` for 'analog input' (3x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_3x; }
};


/*! \brief Modbus `item reference` class of boolean type for 'analog input' (3x-register) memory cell.

    \details Generates code for a boolean Modbus variable of type 'analog output/holding register' (4x-register).
 */

class CnDeviceModbusItem3xBool : public CnDeviceModbusItem3xT<bool>
{
public:
    using CnDeviceModbusItem3xT<bool>::CnDeviceModbusItem3xT;

public:
    /// \details Returns variable size in registers. For boolean always returns `1`.
    virtual uint16_t count() const override { return 1; }
};

typedef CnDeviceModbusItem3xT<int16_t > CnDeviceModbusItem3xInt16 ; //!< Modbus 'analog input' (3x-register) type `Int16`
typedef CnDeviceModbusItem3xT<uint16_t> CnDeviceModbusItem3xUInt16; //!< Modbus 'analog input' (3x-register) type `UInt16`
typedef CnDeviceModbusItem3xT<int32_t > CnDeviceModbusItem3xInt32 ; //!< Modbus 'analog input' (3x-register) type `Int32`
typedef CnDeviceModbusItem3xT<uint32_t> CnDeviceModbusItem3xUInt32; //!< Modbus 'analog input' (3x-register) type `UInt32`
typedef CnDeviceModbusItem3xT<int64_t > CnDeviceModbusItem3xInt64 ; //!< Modbus 'analog input' (3x-register) type `Int64`
typedef CnDeviceModbusItem3xT<uint64_t> CnDeviceModbusItem3xUInt64; //!< Modbus 'analog input' (3x-register) type `UInt64`
typedef CnDeviceModbusItem3xT<float   > CnDeviceModbusItem3xFloat ; //!< Modbus 'analog input' (3x-register) type `Float`
typedef CnDeviceModbusItem3xT<double  > CnDeviceModbusItem3xDouble; //!< Modbus 'analog input' (3x-register) type `Double`

/*! \brief Template `item reference` class for 'analog input' (3x-register) with swapped register order.

    \details Derived from `CnDeviceModbusItemSwappedRegsT`,
    specializing for 'analog input' (3x-register) values.
 */

template <class T>
class CnDeviceModbusItemSwapped3xT : public CnDeviceModbusItemSwappedRegsT<T>
{
public:
    using CnDeviceModbusItemSwappedRegsT<T>::CnDeviceModbusItemSwappedRegsT;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `3x` for 'analog input' (3x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_3x; }
};

typedef CnDeviceModbusItemSwapped3xT<int32_t > CnDeviceModbusItem3xSwappedInt32 ; //!< Modbus 'analog input' (3x-register) swapped-register type `Int32`
typedef CnDeviceModbusItemSwapped3xT<uint32_t> CnDeviceModbusItem3xSwappedUInt32; //!< Modbus 'analog input' (3x-register) swapped-register type `UInt32`
typedef CnDeviceModbusItemSwapped3xT<int64_t > CnDeviceModbusItem3xSwappedInt64 ; //!< Modbus 'analog input' (3x-register) swapped-register type `Int64`
typedef CnDeviceModbusItemSwapped3xT<uint64_t> CnDeviceModbusItem3xSwappedUInt64; //!< Modbus 'analog input' (3x-register) swapped-register type `UInt64`
typedef CnDeviceModbusItemSwapped3xT<float   > CnDeviceModbusItem3xSwappedFloat ; //!< Modbus 'analog input' (3x-register) swapped-register type `Float`
typedef CnDeviceModbusItemSwapped3xT<double  > CnDeviceModbusItem3xSwappedDouble; //!< Modbus 'analog input' (3x-register) swapped-register type `Double`


/*! \brief Modbus `item reference` class of byte-array type for 'analog input' (3x-register) memory cell.

    \details Generates common code for all 'analog input' (3x-register) Modbus variables.
 */

class CnDeviceModbusItem3xByteArray : public CnDeviceModbusItemRegsByteArray
{
public:
    using CnDeviceModbusItemRegsByteArray::CnDeviceModbusItemRegsByteArray;

public:
    /// \details  The `access()` method returns `Cn::Access_Read` (read-only).
    Cn::Access access() const override { return Cn::Access_Read; }

    /// \details Returns memory type `3x` for 'analog input' (3x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_3x; }
};


// ------------------------------------------------------------------------------------------
// ---------------------------------- CnDeviceModbusItem4x ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template Modbus `item reference` class of type 'analog output/holding register' (4x-register) for a device/PLC.

    \details Generates common code for all 'analog output/holding register' (4x-register) Modbus variables.
 */

template <class T>
class CnDeviceModbusItem4xT : public CnDeviceModbusItemRegsT<T>
{
public:
    using CnDeviceModbusItemRegsT<T>::CnDeviceModbusItemRegsT;

public:
    /// \details  The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Returns memory type `4x` for 'analog output/holding register' (4x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_4x; }
};

/*! \brief Modbus `item reference` class of boolean type for 'analog output/holding register' (4x-register) memory cell.

    \details Generates code for a boolean Modbus variable of type 'analog output/holding register' (4x-register).
 */

class CnDeviceModbusItem4xBool : public CnDeviceModbusItem4xT<bool>
{
public:
    using CnDeviceModbusItem4xT<bool>::CnDeviceModbusItem4xT;

public:
    /// \details Returns variable size in registers. For boolean always returns `1`.
    virtual uint16_t count() const override { return 1; }
};

typedef CnDeviceModbusItem4xT<int16_t > CnDeviceModbusItem4xInt16 ; //!< Modbus 'analog output/holding register' (4x-register) type `Int16`
typedef CnDeviceModbusItem4xT<uint16_t> CnDeviceModbusItem4xUInt16; //!< Modbus 'analog output/holding register' (4x-register) type `UInt16`
typedef CnDeviceModbusItem4xT<int32_t > CnDeviceModbusItem4xInt32 ; //!< Modbus 'analog output/holding register' (4x-register) type `Int32`
typedef CnDeviceModbusItem4xT<uint32_t> CnDeviceModbusItem4xUInt32; //!< Modbus 'analog output/holding register' (4x-register) type `UInt32`
typedef CnDeviceModbusItem4xT<int64_t > CnDeviceModbusItem4xInt64 ; //!< Modbus 'analog output/holding register' (4x-register) type `Int64`
typedef CnDeviceModbusItem4xT<uint64_t> CnDeviceModbusItem4xUInt64; //!< Modbus 'analog output/holding register' (4x-register) type `UInt64`
typedef CnDeviceModbusItem4xT<float   > CnDeviceModbusItem4xFloat ; //!< Modbus 'analog output/holding register' (4x-register) type `Float`
typedef CnDeviceModbusItem4xT<double  > CnDeviceModbusItem4xDouble; //!< Modbus 'analog output/holding register' (4x-register) type `Double`


/*! \brief Template `item reference` class for 'analog output/holding register' (4x-register) with swapped register order.

    \details Derived from `CnDeviceModbusItemSwappedRegsT`,
    specializing for 'analog output/holding register' (4x-register) values.
 */

template <class T>
class CnDeviceModbusItemSwapped4xT : public CnDeviceModbusItemSwappedRegsT<T>
{
public:
    using CnDeviceModbusItemSwappedRegsT<T>::CnDeviceModbusItemSwappedRegsT;

public:
    /// \details  The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Returns memory type `4x` for 'analog output/holding register' (4x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_4x; }
};

typedef CnDeviceModbusItemSwapped4xT<int32_t > CnDeviceModbusItem4xSwappedInt32 ; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `Int32 `
typedef CnDeviceModbusItemSwapped4xT<uint32_t> CnDeviceModbusItem4xSwappedUInt32; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `UInt32`
typedef CnDeviceModbusItemSwapped4xT<int64_t > CnDeviceModbusItem4xSwappedInt64 ; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `Int64 `
typedef CnDeviceModbusItemSwapped4xT<uint64_t> CnDeviceModbusItem4xSwappedUInt64; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `UInt64`
typedef CnDeviceModbusItemSwapped4xT<float   > CnDeviceModbusItem4xSwappedFloat ; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `Float `
typedef CnDeviceModbusItemSwapped4xT<double  > CnDeviceModbusItem4xSwappedDouble; //!< Modbus 'аналоговий вихід/регістр зберігання' (4x-регістр) зі зміненим порядком регістрів типу `Double`


/*! \brief Modbus `item reference` class of byte-array type for 'analog output/holding register' (4x-register) memory cell.

    \details Generates common code for all 'analog output/holding register' (4x-register) Modbus variables.
 */

class CnDeviceModbusItem4xByteArray : public CnDeviceModbusItemRegsByteArray
{
public:
    using CnDeviceModbusItemRegsByteArray::CnDeviceModbusItemRegsByteArray;

public:
    /// \details  The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Returns memory type `4x` for 'analog output/holding register' (4x-register).
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_4x; }
};

namespace Cn {

CnDeviceModbusItem *createDeviceModbusItem(Cn::DataSuffix type, 
                                           Modbus::MemoryType memType, 
                                           CnDevice *device, 
                                           uint16_t offset, 
                                           uint16_t count,
                                           int period,
                                           const CnString &messageId = CnString());

} // namespace Cn

//typedef CnSharedPointer<CnDeviceModbusItem> CnDeviceModbusItemPtr; //!< Pointer to `CnDeviceModbusItem` object with automatic deletion support

#endif // CNDEVICEMODBUSITEM_H
