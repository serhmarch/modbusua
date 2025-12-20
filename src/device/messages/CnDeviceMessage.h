/*!
 * \file   CnDeviceMessage.h
 * \brief  Класи для роботи з Modbus-повідомленнями об'єкта пристрою/ПЛК.
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDEVICEMESSAGE_H
#define CNDEVICEMESSAGE_H

#include "CnDeviceMessageBase.h"
#include <device/items/CnDeviceModbusItem.h>

class CnDevice;
class CnDeviceModbusItem;

// ------------------------------------------------------------------------------------------
// ------------------------------ CnDeviceMessage ------------------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Базовий клас для роботи з Modbus-повідомленнями об'єкта пристрою/ПЛК.

    \details Клас повідомлення, що відноситься до об'єкта-пристрою/ПЛК, є групою змінних 'item reference', що читаються/записуються одночасно.
    Дані повідомлення вичитуються/записуються із/в віддалений пристрій/ПЛК, оновлюючи при цьому мітку часу і якість даних 'item reference'.
*/
class CnDeviceMessage : public CnDeviceMessageBase
{
public:
    /// \details Конструктор класу.
    /// \param[in]  device    Об'єкт пристрій/ПЛК
    /// \param[in]  pItem     Об'єкт 'item reference', на основі якого створюється Modbus-повідомлення
    /// \param[in]  maxCount  Максимальний розмір (к-сть бітів/регістрів) даного Modbus-повідомлення
    CnDeviceMessage(CnDevice *device, CnDeviceModbusItem *pItem, uint16_t maxCount);

    /// \details Деструктор класу.
    ~CnDeviceMessage();

public:
    /// \details Функція повертає покажчик на поточний пристрій/ПЛК, до якого відноситься даний Modbus Message.
    inline CnDevice *device() const { return m_device; }

    /// \details Функція повертає покажчик на буфер, який містить зручне для спийняття найменування даного Modbus-повідомлення.
    const CnChar* fineName() const override;

    /// \details Функція повертає зміщення першого біта/регістра даного повідомлення (нумерація з нуля).
    uint16_t offset() const override { return m_offset; }

    /// \details Функція повертає кількість бітів/регістрів даного повідомлення.
    uint16_t count() const override { return m_count; }

    /// \details Функція повертає зміщення першого біта/регістра даного повідомлення (нумерація з нуля).
    uint16_t execOffset() const override { return m_execOffset; }

    /// \details Функція повертає кількість бітів/регістрів даного повідомлення.
    uint16_t execCount() const override { return m_execCount; }

    /// \details Функція повертає період надсилання повідолмення.
    int period() const override { return m_period; }

    /// \details Функція повертає покажчик на внутрішній буфер Modbus-повідомлення.
    void* innerBuffer() override { return m_buff.data(); }

    /// \details Функція повертає розмір внутрішнього буфер Modbus-повідомлення, що вимірюється кількістю байт.
    uint32_t innerBufferSize() const override { return static_cast<uint32_t>(m_buff.size()); }

    /// \details Функція повертає розмір внутрішнього буфер Modbus-повідомлення, що вимірюється кількістю біт.
    inline uint32_t innerBufferBitSize() const { return innerBufferSize() * MB_BYTE_SZ_BITES; }

    /// \details Функція повертає розмір внутрішнього буфер Modbus-повідомлення, що вимірюється кількістю регістрів.
    inline uint32_t innerBufferRegSize() const { return innerBufferSize() / MB_REGE_SZ_BYTES; }

    /// \details Функція повертає статус OPC_QUALITY (DASToolkit) Modbus-повідомлення.
    Cn::StatusCode status() const override { return m_status; }

    /// \details Функція повертає мітку часу, коли Modbus-повідомлення було останній раз отримане або зафіксований неуспішний результат.
    CnTimestamp timestamp() const override { return m_timestamp; }

    /// \details Функція повертає мітку часу, коли Modbus-повідомлення було відправлене у мережу.
    CnTimestamp timestampBegin() const override { return m_timestampBegin; }

public:
    /// \details Функція викликається перед початком процесу відправки Modbus-повідомлення.
    void beginProcess() override;

    /// \details Функція викликається після завершення обробки Modbus-повідомлення, встановлюючи якість `status` і мітку часу `timestamp`.
    void setComplete(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err = CnString()) override;

    /// \details Функція викликається після завершення обробки Modbus-повідомлення, встановлюючи якість `status` і мітку часу `timestamp`.
    virtual void setComplete(Cn::StatusCode status, CnTimestamp timestamp);

    /// \details Функція копіює дані із внутрішнього буфера даного Modbus-повідомлення у буфер `buff`.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    bool getData(uint16_t innerOffset, uint16_t count, void* buff) const override { CnCriticalSectionLocker _(&m_cs); return getDataInner(innerOffset, count, buff); }

    /// \details Функція встановлює дані із буфера `buff` у внутрішній буфер даного Modbus-повідомлення.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    bool setData(uint16_t innerOffset, uint16_t count, const void* buff) override { CnCriticalSectionLocker _(&m_cs); return setDataInner(innerOffset, count, buff); }

public:
    /// \details Функція повертає посилання на константний ідентифікатор даного Modbus-повідомлення. 
    /// Він задається значенням `pItem->messageId()` у конструкторі,
    inline const CnString &messageId() const { return m_messageId;  }

    /// \details
    uint32_t itemCount() const override { CnCriticalSectionLocker _(&m_cs); return static_cast<uint32_t>(m_items.size()); }

    /// \details
    inline const CnList<CnDeviceModbusItem*> &items() const { return m_items; }

public: // statistics
    inline CnString StatRange() const { return fineName(); } ///< \details Статистика. Текстове представлення діапазону адрес Modbus-посилки

    inline uint32_t            StatGoodCount           () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.GoodCount           ; } ///< \details Статистика. Загальна кількість успішних Modbus-посилок
    inline uint32_t            StatBadCount            () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.BadCount            ; } ///< \details Статистика. Загальна кількість неуспішних Modbus-посилок
    inline Modbus::StatusCode  StatStatus              () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.Status              ; } ///< \details Статистика. Статус останньої Modbus-посилки (п.2.9)
    inline CnTimestamp  StatTimestamp           () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.Timestamp           ; } ///< \details Статистика. Час (PC) останньої Modbus-посилки
    inline CnTimestamp  StatLastSuccessTimestamp() const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.LastSuccessTimestamp; } ///< \details Статистика. Час (PC) останньої успішної Modbus-посилки
    inline Modbus::StatusCode  StatLastErrorStatus     () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.LastErrorStatus     ; } ///< \details Статистика. Статус останньої помилки Modbus-посилки (п.2.9)
    inline CnTimestamp  StatLastErrorTimestamp  () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.LastErrorTimestamp  ; } ///< \details Статистика. Час (PC) останньої неуспішної Modbus-посилки
    inline CnString       StatLastErrorText       () const { CnCriticalSectionLocker _(&m_stat.cs); return m_stat.LastErrorText       ; } ///< \details Статистика. Текстове представлення останньої помилки Modbus-посилки

protected:
    /// \details Функція копіює дані із внутрішнього буфера даного Modbus-повідомлення у буфер `buff`.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    virtual bool getDataInner(uint16_t innerOffset, uint16_t count, void* buff) const = 0;

    /// \details Функція встановлює дані із буфера `buff` у внутрішній буфер даного Modbus-повідомлення.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    virtual bool setDataInner(uint16_t innerOffset, uint16_t count, const void* buff) = 0;

    /// \details Функція статистичні дані даного об'єкта повідомлення.
    /// \param[in]  status      Поточний статус повідомлення
    /// \param[in]  timestamp   Поточна мітка часу повідомлення
    /// \param[in]  err         Рядок помилки у разі помилки
    void setStatistics(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err);

protected:
    void addItemInner(CnDeviceModbusItem* pItem);
    void removeItemInner(CnDeviceModbusItem* pItem);
    void writeToItems();
    void readFromItems();

protected:
    mutable CnCriticalSection m_cs;
    CnDevice *m_device;
    uint16_t m_offset;
    uint16_t m_count;
    uint16_t m_execOffset;
    uint16_t m_execCount;
    uint16_t m_maxCount;
    int m_period;
    std::vector<uint8_t> m_buff;
    Cn::StatusCode m_status;
    CnTimestamp m_timestamp;
    CnTimestamp m_timestampBegin;
    mutable CnChar m_fineName[14];
    CnString m_messageId;
    
    typedef CnList<CnDeviceModbusItem*> Items_t;
    Items_t m_items;

    struct
    {
        mutable CnCriticalSection cs;
        uint32_t GoodCount;
        uint32_t BadCount;
        Modbus::StatusCode Status;
        CnTimestamp Timestamp;
        CnTimestamp LastSuccessTimestamp;
        Modbus::StatusCode LastErrorStatus;
        CnTimestamp LastErrorTimestamp;
        CnString LastErrorText;
    } m_stat;
    
};

// ------------------------------------------------------------------------------------------
// ----------------------------- CnDevicePollMessage -----------------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Базовий клас для роботи з Modbus-повідомленнями читання для об'єкта пристрою/ПЛК.

    \details Клас повідомлення, що відноситься до об'єкта-пристрою/ПЛК, є групою змінних 'item reference', що читаються одночасно.
    Дані повідомлення вичитуються із віддаленого пристрою/ПЛК, оновлюючи при цьому мітку часу і якість даних 'item reference'.
*/
class CnDevicePollMessage : public CnDeviceMessage
{
public:
    CnDevicePollMessage(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);
    ~CnDevicePollMessage();

public:
    void initialize() override;
    void finalize() override;

public: // CnDeviceMessage interface
    bool addItem(CnDeviceModbusItem* item) override;
    void setComplete(Cn::StatusCode status, CnTimestamp timestamp) override;
    void clearItems() override;
};


// ------------------------------------------------------------------------------------------
// ------------------------------ CnDevicePokeMessage ------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief  Базовий клас для роботи з Modbus-повідомленнями запису для об'єкта пристрою/ПЛК.

    \details Клас повідомлення, що відноситься до об'єкта-пристрою/ПЛК, є групою змінних 'item reference', що записуються одночасно.
    Дані повідомлення записуються у віддалений пристрій/ПЛК, оновлюючи при цьому мітку часу і якість даних 'item reference'.
*/
class CnDevicePokeMessage : public CnDeviceMessage
{
public:
    CnDevicePokeMessage(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);
    ~CnDevicePokeMessage();
    
public:
    void initialize() override;
    void finalize() override;

public: // CnDeviceMessage interface
    bool addItem(CnDeviceModbusItem* item) override;
    void beginProcess() override;
    void clearItems() override;

};


// ------------------------------------------------------------------------------------------
// -------------------------- CnDevicePollMessageReadCoils -------------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Клас для роботи з Modbus-повідомленнями читання дискретних виходів (котушок, 0x-бітів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePollMessageReadCoils : public CnDevicePollMessage
{
public:
    CnDevicePollMessageReadCoils(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_0x; }
    int ModbusFunction() const override { return MBF_READ_COILS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};


// ------------------------------------------------------------------------------------------
// ---------------------- CnDevicePollMessageReadDiscreteInputs --------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Клас для роботи з Modbus-повідомленнями читання дискретних входів (1x-бітів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePollMessageReadDiscreteInputs : public CnDevicePollMessage
{
public:
    CnDevicePollMessageReadDiscreteInputs(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_1x; }
    int ModbusFunction() const override { return MBF_READ_DISCRETE_INPUTS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};


// ------------------------------------------------------------------------------------------
// --------------------- CnDevicePollMessageReadInputRegisters ---------------------
// ------------------------------------------------------------------------------------------

/*!  * \brief  Клас для роботи з Modbus-повідомленнями читання аналогових входів (3x-регістрів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePollMessageReadInputRegisters : public CnDevicePollMessage
{
public:
    CnDevicePollMessageReadInputRegisters(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_3x; }
    int ModbusFunction() const override { return MBF_READ_INPUT_REGISTERS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};


// ------------------------------------------------------------------------------------------
// -------------------- CnDevicePollMessageReadHoldingRegisters --------------------
// ------------------------------------------------------------------------------------------

/*!  * \brief  Клас для роботи з Modbus-повідомленнями читання аналогових виходів (регістрів зберігання, 4x-регістрів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePollMessageReadHoldingRegisters : public CnDevicePollMessage
{
public:
    CnDevicePollMessageReadHoldingRegisters(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_4x; }
    int ModbusFunction() const override { return MBF_READ_HOLDING_REGISTERS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};


// ------------------------------------------------------------------------------------------
// --------------------- CnDevicePokeMessageWriteMultipleCoils ---------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Клас для роботи з Modbus-повідомленнями запису дискретних виходів (котушок, 0x-бітів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePokeMessageWriteMultipleCoils : public CnDevicePokeMessage
{
public:
    CnDevicePokeMessageWriteMultipleCoils(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_0x; }
    int ModbusFunction() const override { return MBF_WRITE_MULTIPLE_COILS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};


// ------------------------------------------------------------------------------------------
// ------------------- CnDevicePokeMessageWriteMultipleRegisters -------------------
// ------------------------------------------------------------------------------------------

/*!  * \brief  Клас для роботи з Modbus-повідомленнями запису аналогових виходів (регістрів зберігання, 4x-регістрів) для об'єкта пристрою/ПЛК.
*/
class CnDevicePokeMessageWriteMultipleRegisters : public CnDevicePokeMessage
{
public:
    CnDevicePokeMessageWriteMultipleRegisters(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount);

public: // CnDeviceMessage interface
    Modbus::MemoryType memoryType() const override { return Modbus::Memory_4x; }
    int ModbusFunction() const override { return MBF_WRITE_MULTIPLE_REGISTERS; }

protected:
    bool getDataInner(uint16_t innerOffset, uint16_t count, void *buff) const override;
    bool setDataInner(uint16_t innerOffset, uint16_t count, const void *buff) override;
};

namespace Cn {

/// \details
CnDevicePollMessage *createDeviceModbusPollMessage(CnDevice *device, CnDeviceModbusItem *item);

/// \details
CnDevicePokeMessage *createDeviceModbusPokeMessage(CnDevice *device, CnDeviceModbusItem *item);

} // namespace Cn


#endif // CNDEVICEMODBUSMESSAGE_H