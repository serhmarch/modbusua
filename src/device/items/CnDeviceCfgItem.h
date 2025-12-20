/*! 
 * \file   CnDeviceCfgItem.h
 * \brief  Defines configuration `item reference` list for a device/PLC
 *
 * \author serhmarch
 * \date   February 2024
 */
#ifndef CNDEVICECFGITEM_H
#define CNDEVICECFGITEM_H

#include "CnDeviceBaseItem.h"
#include <device/CnDevice.h>
#include <device/CnDeviceConfig.h>


/*! \brief Base configuration `item reference` class for a device/PLC.

    \details Combines common properties inherent to all configuration variables,
    for example, all configuration variables are read-only, i.e.
    the `AccessRights()` method returns the OPC_READABLE flag.
 */

class CnDeviceCfgItem : public CnDeviceBaseItem
{
public:
    CnDeviceCfgItem(CnDevice *device) : CnDeviceBaseItem(device), m_cfg(device->cfg()) {};

public:
    /// \details Returns the `ItemType` of this ItemReference.
    ItemType itemType() const override { return Item_Config; }

protected:
    CnDeviceConfig *m_cfg;
};

/*! \brief Template configuration `item reference` class for a device/PLC.

    \details Generates code common to all configuration variables.
    Differs only by the `CnDevice` class method that returns the value of the corresponding configuration element.
    The configuration read code is implemented in the `ReadValue()` method.\n
    The class method responsible for returning the value in `ReadValue()` has the form `R CnDevice::PMethod()`,
    which is passed as a template parameter as a pointer to method `R (CnDevice::*PMethod)()`.
 */

template <class R, R (CnDeviceConfig::*PMethod)() const>
class CnDeviceCfgItemBase : public CnDeviceCfgItem
{
public:
    using CnDeviceCfgItem::CnDeviceCfgItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details Implements reading the configuration value by calling `CnDeviceConfig::*PMethod()`,
    /// a pointer to which is passed as a template parameter.
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        R v = (m_cfg->*PMethod)();
        //if (status)
        //    *status = Cn::Status_Good;
        //if (timestamp)
        //    *timestamp = Cn::currentTimestamp();
        return CnVariant(v);
    }
};

/// \details Проміжний шаблон функції, єдиним призначення якого є визначення типу повернення функції.
template <class RT> inline RT getDeviceCfgMethodRT(RT(CnDeviceConfig::*)()const) { return RT(); }

#define DEVICE_CFG_ITEM_TYPE(name) typedef CnDeviceCfgItemBase<decltype(getDeviceCfgMethodRT(&CnDeviceConfig::Cfg##name)), &CnDeviceConfig::Cfg##name>

DEVICE_CFG_ITEM_TYPE(Name                     ) CnDeviceCfgItemName                     ; ///< Configuration. Name of this device/PLC
DEVICE_CFG_ITEM_TYPE(EnableDevice             ) CnDeviceCfgItemEnableDevice             ; ///< Configuration. Availability of this device/PLC in execution sequence (1-on, 0-off)
DEVICE_CFG_ITEM_TYPE(ModbusUnit               ) CnDeviceCfgItemModbusUnit               ; ///< Configuration. Modbus address of this device/PLC
DEVICE_CFG_ITEM_TYPE(RepeatCount              ) CnDeviceCfgItemRepeatCount              ; ///< Configuration. Maximum number of retries for a Modbus request upon failure
DEVICE_CFG_ITEM_TYPE(RestoreTimeout           ) CnDeviceCfgItemRestoreTimeout           ; ///< Configuration. Timeout to restore communication after error (excluded from execution sequence) (milliseconds)
DEVICE_CFG_ITEM_TYPE(MaxReadCoils             ) CnDeviceCfgItemMaxReadCoils             ; ///< Configuration. Maximum number of Coils to read in one message
DEVICE_CFG_ITEM_TYPE(MaxWriteMultipleCoils    ) CnDeviceCfgItemMaxWriteMultipleCoils    ; ///< Configuration. Maximum number of Coils to write in one message
DEVICE_CFG_ITEM_TYPE(MaxReadDiscreteInputs    ) CnDeviceCfgItemMaxReadDiscreteInputs    ; ///< Configuration. Maximum number of DiscreteInputs to read in one message
DEVICE_CFG_ITEM_TYPE(MaxReadInputRegisters    ) CnDeviceCfgItemMaxReadInputRegisters    ; ///< Configuration. Maximum number of InputRegisters to read in one message
DEVICE_CFG_ITEM_TYPE(MaxReadHoldingRegisters  ) CnDeviceCfgItemMaxReadHoldingRegisters  ; ///< Configuration. Maximum number of HoldingRegisters to read in one message
DEVICE_CFG_ITEM_TYPE(MaxWriteMultipleRegisters) CnDeviceCfgItemMaxWriteMultipleRegisters; ///< Configuration. Maximum number of HoldingRegisters to write in one message
DEVICE_CFG_ITEM_TYPE(DefaultPeriod            ) CnDeviceCfgItemDefaultPeriod            ; ///< Configuration. Default polling period for each Modbus Item
DEVICE_CFG_ITEM_TYPE(RequestTimeout           ) CnDeviceCfgItemRequestTimeout           ; ///< Configuration. Request timeout for each Modbus Item, after which polling of that item stops
DEVICE_CFG_ITEM_TYPE(LogFlags                 ) CnDeviceCfgItemLogFlags                 ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogOutput                ) CnDeviceCfgItemLogOutput                ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogFileFormat            ) CnDeviceCfgItemLogFileFormat            ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogFileTimeformat        ) CnDeviceCfgItemLogFileTimeformat        ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogFilePath              ) CnDeviceCfgItemLogFilePath              ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogFileMaxCount          ) CnDeviceCfgItemLogFileMaxCount          ; ///< Configuration.
DEVICE_CFG_ITEM_TYPE(LogFileMaxSize           ) CnDeviceCfgItemLogFileMaxSize           ; ///< Configuration.


#endif // CNDEVICECFGITEM_H