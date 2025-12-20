#include "CnDeviceModbusItem.h"

#include <device/CnDevice.h>

// ------------------------------------------------------------------------------------------
// ---------------------------------- CnDeviceModbusItem ---------------------------------
// ------------------------------------------------------------------------------------------

CnDeviceModbusItem::CnDeviceModbusItem(CnDevice *device, uint16_t offset, int period, const CnString &messageId) :
    CnDeviceBaseItem(device),
    m_offset(offset),
    m_period(period),
    m_messageId(messageId),
    m_message(nullptr)
{
    m_status = Cn::Status_Uncertain;
    m_timestamp = Cn::currentTimestamp();
    m_timestampLastRequest = 0;
}

void CnDeviceModbusItem::setMessage(CnDeviceMessage *message)
{
    CnCriticalSectionLocker _(&m_cs); 
    m_message = message;
    //resetInner();
}

void CnDeviceModbusItem::processDataUnsafe()
{
}

void CnDeviceModbusItem::update(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp)
{
    CnCriticalSectionLocker _(&m_cs);
    updateInner(value, status, timestamp);  
}

void CnDeviceModbusItem::resetInner()
{
    //if (m_message)
    //    m_status = Cn::Status_Uncertain;
    //else
    //    m_status = Cn::Status_Bad;
    m_status = Cn::Status_Bad;
    m_timestamp = Cn::currentTimestamp();
}

// ------------------------------------------------------------------------------------------
// ----------------------------- CnDeviceModbusItemByteArray -----------------------------
// ------------------------------------------------------------------------------------------

CnVariant CnDeviceModbusItemByteArray::value(Cn::StatusCode *status, CnTimestamp *timestamp) const
{
    CnCriticalSectionLocker _(&m_cs);        
    if (status)
        *status = statusInner();
    if (timestamp)
        *timestamp = m_timestamp;
    if (Cn::StatusIsUncertain(this->statusInner()))
        return CnVariant();
    return CnVariant(this->toString());
}

void CnDeviceModbusItemByteArray::updateInner(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp)
{
    m_status = status;
    m_timestamp = timestamp;
    if (!value.isNull())
        fillByteArray(value.toString(), m_data);
}

CnString CnDeviceModbusItemByteArray::toString() const
{
    CnString s;
    const uint8_t *buff = reinterpret_cast<const uint8_t*>(m_data.data());
    uint16_t szBuff = static_cast<uint16_t>(m_data.size());
    s.resize(szBuff*2);
    for (int i = 0; i < szBuff; i++)
    {
        uint8_t v = buff[i];
        uint8_t vHi = (v >> 4);
        uint8_t vLo = v & 0xF;
        s[i*2  ] = (vHi <= 9) ? CnCHR('0') + vHi : CnCHR('A') + (vHi-10);
        s[i*2+1] = (vLo <= 9) ? CnCHR('0') + vLo : CnCHR('A') + (vLo-10);
    }
    return s;
}

bool CnDeviceModbusItemByteArray::fillByteArray(const CnString &s, uint8_t *buff, uint16_t szBuff) const
{
    uint16_t sz = static_cast<uint16_t>(s.size() / 2);
    if (sz > szBuff)
        sz = szBuff;
    for (int i = 0; i < sz; i++)
    {
        uint8_t vHi, vLo;
        CnChar c = s[i * 2];
        if ((c >= CnCHR('0')) && (c <= CnCHR('9')))
            vHi = c - CnCHR('0');
        else if ((c >= CnCHR('A')) && (c <= CnCHR('F')))
            vHi = c - CnCHR('A') + 10;
        else
        {
            //m_device->logWarning(CnSTR("%s. Wrong Hex-ByteArray format"), this->name().data());
            return false;
        }

        c = s[i*2+1];
        if ((c >= CnCHR('0')) && (c <= CnCHR('9')))
            vLo = c - CnCHR('0');
        else if ((c >= CnCHR('A')) && (c <= CnCHR('F')))
            vLo = c - CnCHR('A') + 10;
        else
        {
            //m_device->logWarning(CnSTR("%s. Wrong Hex-ByteArray format"), this->name().data());
            return false;
        }
        buff[i] = (vHi << 4) | vLo;
    }
    return true;
}

CnDeviceModbusItemBitsByteArray::CnDeviceModbusItemBitsByteArray(CnDevice *device, uint16_t offset, uint16_t count, int period, const CnString &messageId) :
    CnDeviceModbusItemByteArray(device, offset, period, messageId)
{
    m_count = count;
    m_data.resize((count + 7) / 8);
}

CnDeviceModbusItemRegsByteArray::CnDeviceModbusItemRegsByteArray(CnDevice *device, uint16_t offset, uint16_t count, int period, const CnString &messageId) :
    CnDeviceModbusItemByteArray(device, offset, period, messageId)
{
    m_data.resize(count * MB_REGE_SZ_BYTES);
}

namespace Cn {

CnDeviceModbusItem *createDeviceModbusItem(Cn::DataSuffix type, Modbus::MemoryType memType, CnDevice *device, uint16_t offset, uint16_t count, int period, const CnString &messageId)
{
    switch (memType)
    {
    case Modbus::Memory_0x:
        switch (type)
        {
        case Cn::Suffix_Default:
            return new CnDeviceModbusItem0xBool(device, offset, period, messageId);
        case Cn::Suffix_Int16:
            return new CnDeviceModbusItem0xInt16(device, offset, period, messageId);
        case Cn::Suffix_UInt16:
            return new CnDeviceModbusItem0xUInt16(device, offset, period, messageId);
        case Cn::Suffix_Int32:
            return new CnDeviceModbusItem0xInt32(device, offset, period, messageId);
        case Cn::Suffix_UInt32:
            return new CnDeviceModbusItem0xUInt32(device, offset, period, messageId);
        case Cn::Suffix_Int64:
            return new CnDeviceModbusItem0xInt64(device, offset, period, messageId);
        case Cn::Suffix_UInt64:
            return new CnDeviceModbusItem0xUInt64(device, offset, period, messageId);
        case Cn::Suffix_Float:
            return new CnDeviceModbusItem0xFloat(device, offset, period, messageId);
        case Cn::Suffix_Double:
            return new CnDeviceModbusItem0xDouble(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt32:
            return new CnDeviceModbusItem0xSwappedInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt32:
            return new CnDeviceModbusItem0xSwappedUInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt64:
            return new CnDeviceModbusItem0xSwappedInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt64:
            return new CnDeviceModbusItem0xSwappedUInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedFloat:
            return new CnDeviceModbusItem0xSwappedFloat(device, offset, period, messageId);
        case Cn::Suffix_SwappedDouble:
            return new CnDeviceModbusItem0xSwappedDouble(device, offset, period, messageId);
        case Cn::Suffix_ByteArray:
            return new CnDeviceModbusItem0xByteArray(device, offset, count, period, messageId);
        default:
            return nullptr;
        }
        break;
    case Modbus::Memory_1x:
        switch (type)
        {
        case Cn::Suffix_Default:
            return new CnDeviceModbusItem1xBool(device, offset, period, messageId);
        case Cn::Suffix_Int16:
            return new CnDeviceModbusItem1xInt16(device, offset, period, messageId);
        case Cn::Suffix_UInt16:
            return new CnDeviceModbusItem1xUInt16(device, offset, period, messageId);
        case Cn::Suffix_Int32:
            return new CnDeviceModbusItem1xInt32(device, offset, period, messageId);
        case Cn::Suffix_UInt32:
            return new CnDeviceModbusItem1xUInt32(device, offset, period, messageId);
        case Cn::Suffix_Int64:
            return new CnDeviceModbusItem1xInt64(device, offset, period, messageId);
        case Cn::Suffix_UInt64:
            return new CnDeviceModbusItem1xUInt64(device, offset, period, messageId);
        case Cn::Suffix_Float:
            return new CnDeviceModbusItem1xFloat(device, offset, period, messageId);
        case Cn::Suffix_Double:
            return new CnDeviceModbusItem1xDouble(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt32:
            return new CnDeviceModbusItem1xSwappedInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt32:
            return new CnDeviceModbusItem1xSwappedUInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt64:
            return new CnDeviceModbusItem1xSwappedInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt64:
            return new CnDeviceModbusItem1xSwappedUInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedFloat:
            return new CnDeviceModbusItem1xSwappedFloat(device, offset, period, messageId);
        case Cn::Suffix_SwappedDouble:
            return new CnDeviceModbusItem1xSwappedDouble(device, offset, period, messageId);
        case Cn::Suffix_ByteArray:
            return new CnDeviceModbusItem1xByteArray(device, offset, count, period, messageId);
        default:
            return nullptr;
        }
        break;
    case Modbus::Memory_3x:
        switch (type)
        {
        case Cn::Suffix_Int16:
            return new CnDeviceModbusItem3xInt16(device, offset, period, messageId);
        case Cn::Suffix_UInt16:
        case Cn::Suffix_Default:
            return new CnDeviceModbusItem3xUInt16(device, offset, period, messageId);
        case Cn::Suffix_Int32:
            return new CnDeviceModbusItem3xInt32(device, offset, period, messageId);
        case Cn::Suffix_UInt32:
            return new CnDeviceModbusItem3xUInt32(device, offset, period, messageId);
        case Cn::Suffix_Int64:
            return new CnDeviceModbusItem3xInt64(device, offset, period, messageId);
        case Cn::Suffix_UInt64:
            return new CnDeviceModbusItem3xUInt64(device, offset, period, messageId);
        case Cn::Suffix_Float:
            return new CnDeviceModbusItem3xFloat(device, offset, period, messageId);
        case Cn::Suffix_Double:
            return new CnDeviceModbusItem3xDouble(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt32:
            return new CnDeviceModbusItem3xSwappedInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt32:
            return new CnDeviceModbusItem3xSwappedUInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt64:
            return new CnDeviceModbusItem3xSwappedInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt64:
            return new CnDeviceModbusItem3xSwappedUInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedFloat:
            return new CnDeviceModbusItem3xSwappedFloat(device, offset, period, messageId);
        case Cn::Suffix_SwappedDouble:
            return new CnDeviceModbusItem3xSwappedDouble(device, offset, period, messageId);
        case Cn::Suffix_ByteArray:
            return new CnDeviceModbusItem3xByteArray(device, offset, count, period, messageId);
        default:
            return nullptr;
        }
        break;
    case Modbus::Memory_4x:
        switch (type)
        {
        case Cn::Suffix_Int16:
            return new CnDeviceModbusItem4xInt16(device, offset, period, messageId);
        case Cn::Suffix_UInt16:
        case Cn::Suffix_Default:
            return new CnDeviceModbusItem4xUInt16(device, offset, period, messageId);
        case Cn::Suffix_Int32:
            return new CnDeviceModbusItem4xInt32(device, offset, period, messageId);
        case Cn::Suffix_UInt32:
            return new CnDeviceModbusItem4xUInt32(device, offset, period, messageId);
        case Cn::Suffix_Int64:
            return new CnDeviceModbusItem4xInt64(device, offset, period, messageId);
        case Cn::Suffix_UInt64:
            return new CnDeviceModbusItem4xUInt64(device, offset, period, messageId);
        case Cn::Suffix_Float:
            return new CnDeviceModbusItem4xFloat(device, offset, period, messageId);
        case Cn::Suffix_Double:
            return new CnDeviceModbusItem4xDouble(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt32:
            return new CnDeviceModbusItem4xSwappedInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt32:
            return new CnDeviceModbusItem4xSwappedUInt32(device, offset, period, messageId);
        case Cn::Suffix_SwappedInt64:
            return new CnDeviceModbusItem4xSwappedInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedUInt64:
            return new CnDeviceModbusItem4xSwappedUInt64(device, offset, period, messageId);
        case Cn::Suffix_SwappedFloat:
            return new CnDeviceModbusItem4xSwappedFloat(device, offset, period, messageId);
        case Cn::Suffix_SwappedDouble:
            return new CnDeviceModbusItem4xSwappedDouble(device, offset, period, messageId);
        case Cn::Suffix_ByteArray:
            return new CnDeviceModbusItem4xByteArray(device, offset, count, period, messageId);
        default:
            return nullptr;
        }
    }
    return nullptr;
}

} // namespace Cn


