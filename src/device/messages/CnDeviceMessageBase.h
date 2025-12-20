#ifndef CNDEVICEMESSAGEBASE
#define CNDEVICEMESSAGEBASE

#include <CnGlobal.h>
#include <CnObject.h>

class CnDeviceModbusItem;

class CnDeviceMessageBase : public CnObject
{
public:
    /// \details Віртуальна функція викликається перед початком обробки об'єкта.
    virtual void initialize() = 0;

    /// \details Віртуальна функція викликається перед початком видалення об'єкта.
    virtual void finalize() = 0;

    /// \details Функція повертає покажчик на буфер, який містить зручне для спийняття найменування даного Modbus-повідомлення.
    virtual const CnChar* fineName() const = 0;

    /// \details Функція повертає зміщення першого біта/регістра даного повідомлення (нумерація з нуля).
    virtual uint16_t offset() const = 0;

    /// \details Функція повертає зміщення першого біта/регістра даного повідомлення (нумерація з нуля).
    virtual uint16_t execOffset() const = 0;

    /// \details Функція повертає кількість бітів/регістрів даного повідомлення.
    virtual uint16_t count() const = 0;

    /// \details Функція повертає кількість бітів/регістрів даного повідомлення.
    virtual uint16_t execCount() const = 0;

    /// \details Функція повертає період надсилання повідолмення.
    virtual int period() const = 0;

    /// \details Функція повертає покажчик на внутрішній буфер Modbus-повідомлення.
    virtual void* innerBuffer() = 0;

    /// \details Функція повертає розмір внутрішнього буфер Modbus-повідомлення, що вимірюється кількістю байт.
    virtual uint32_t innerBufferSize() const = 0;

    /// \details Функція повертає статус `Cn::StatusCode` Modbus-повідомлення.
    virtual Cn::StatusCode status() const = 0;

    /// \details Функція повертає мітку часу, коли Modbus-повідомлення було останній раз отримане або зафіксований неуспішний результат.
    virtual CnTimestamp timestamp() const = 0;

    /// \details Функція повертає мітку часу, коли Modbus-повідомлення було відправлене у мережу.
    virtual CnTimestamp timestampBegin() const = 0;

    /// \details Функція викликається перед початком процесу відправки Modbus-повідомлення.
    virtual void beginProcess() = 0;

    /// \details Функція вираховує чи може елемент `pItem` бути доданим до даного Modbus-повідомлення.
    /// При цьому якщо `pItem` можна додати, то можуть змінитися параметри повідомлення, такі як початкове зміщення та к-сть бітів регістрів.\n 
    /// Функція повертає `true`, якщо елемент `pItem` може бути доданий до повідомлення, `false` - в іншому випадку.
    virtual bool addItem(CnDeviceModbusItem* item) = 0;

    /// \details Функція викликається після завершення обробки Modbus-повідомлення, встановлюючи якість `status` і мітку часу `timestamp`.
    virtual void setComplete(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err = CnString()) = 0;

    /// \details Функція повертає тип пам'яті, який відноситься до даного Modbus-повідомлення.
    virtual Modbus::MemoryType memoryType() const = 0;

    /// \details Функція повертає номер Modbus-функції, який відноситься до даного Modbus-повідомлення.
    virtual int ModbusFunction() const = 0;

    /// \details Функція копіює дані із внутрішнього буфера даного Modbus-повідомлення у буфер `buff`.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    virtual bool getData(uint16_t innerOffset, uint16_t count, void* buff) const = 0;

    /// \details Функція встановлює дані із буфера `buff` у внутрішній буфер даного Modbus-повідомлення.
    /// \param[in]  innerOffset Зміщення біта/регістру у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  count       Кількість бітів/регістрів у внутрішньому буфері Modbus-повідомлення
    /// \param[in]  buff        Покажчик на буфер, куди записується результат
    virtual bool setData(uint16_t innerOffset, uint16_t count, const void* buff) = 0;

    /// \details
    virtual uint32_t itemCount() const = 0;

    /// \details
    virtual void clearItems() = 0;

public: // signals
    /// \details
    void signalRemovingItem(CnDeviceModbusItem* item);
};

#endif // CNDEVICEMESSAGEBASE