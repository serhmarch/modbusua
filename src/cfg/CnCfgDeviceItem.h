/*!
 * \file   CnCfgDeviceItem.h
 * \brief  Задає структуру конфігурації `item reference` для пристрою/ПЛК у пам'яті
 *
 * \author serhmarch
 * \date   August 2025
 */
#ifndef CNCFGDEVICEITEM_H
#define CNCFGDEVICEITEM_H

#include <CnGlobal.h>
#include <device/items/CnDeviceBaseItem.h>

/*! \brief Configuration item ("item reference") for a device/PLC.

    \details `CnCfgDeviceItem` encapsulates the parsed attributes of
    a device item: item type, data suffix, Modbus memory type, base
    offset and register offset, element count, polling period,
    message identifier, and an optional list of bit descriptors.
    Instances represent a normalized, in-memory view of the source
    "item reference" string.

    \sa `CnCfgDeviceItemParser`, `CnDeviceBaseItem`
*/
class CnCfgDeviceItem
{
public:
    /// \details Constructs a device item with type and data suffix.
    CnCfgDeviceItem(CnDeviceBaseItem::ItemType type, Cn::DataSuffix dataSuffix) :
        m_type(type),
        m_dataSuffix(dataSuffix),
        m_memoryType(Modbus::Memory_Unknown),
        m_offset(0),
        m_count(0),
        m_period(CN_MODBUS_PERIOD_DEFAULT_VALUE)
        {}

public:
    /// \details Returns the item name.
    inline const CnString& name() const { return m_name; }

    /// \details Sets the item name.
    inline void setName(const CnString& v) { m_name = v; }

    /// \details Returns the original "item reference" string.
    inline const CnString& itemReference() const { return m_itemReference; }

    /// \details Sets the original "item reference" string.
    inline void setItemReference(const CnString& v) { m_itemReference = v; }

    /// \details Returns the item type.
    inline CnDeviceBaseItem::ItemType type() const { return m_type; }

    /// \details Sets the item type.
    inline void setType(CnDeviceBaseItem::ItemType v) { m_type = v; }

    /// \details Returns the data suffix (format/kind).
    inline Cn::DataSuffix dataSuffix() const { return m_dataSuffix; }

    /// \details Sets the data suffix (format/kind).
    inline void setDataSuffix(Cn::DataSuffix v) { m_dataSuffix = v; }

    /// \details Returns the Modbus memory type for this item.
    inline Modbus::MemoryType memoryType() const { return m_memoryType; }

    /// \details Sets the Modbus memory type for this item.
    inline void setMemoryType(Modbus::MemoryType v) { m_memoryType = v; }

    /// \details Returns the base table offset.
    inline uint16_t offset() const { return m_offset; }

    /// \details Sets the base table offset.
    inline void setOffset(uint16_t offset) { m_offset = offset; }

    /// \details Returns the element count or register width.
    inline uint16_t count() const { return m_count; }

    /// \details Sets the element count or register width.
    inline void setCount(uint16_t v) { m_count = v; }

    /// \details Returns the polling period (milliseconds).
    inline int period() const { return m_period; }

    /// \details Sets the polling period (milliseconds).
    inline void setPeriod(int v) { m_period = v; }

    /// \details Returns an optional message identifier.
    inline const CnString& messageId() const { return m_messageId; }

    /// \details Sets an optional message identifier.
    inline void setMessageId(const CnString& v) { m_messageId = v; }

private:
    CnString m_name;
    CnString m_itemReference;
    CnDeviceBaseItem::ItemType m_type;
    Cn::DataSuffix m_dataSuffix;
    Modbus::MemoryType m_memoryType;
    uint16_t m_offset;
    uint16_t m_count;
    int m_period;
    CnString m_messageId;
};

#endif // CNCFGDEVICEITEM_H
