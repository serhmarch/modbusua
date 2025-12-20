/*! 
 * \file   CnDeviceSetItem.h
 * \brief  Defines settings `item reference` list for a device/PLC
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNDEVICESETITEM_H
#define CNDEVICESETITEM_H

#include "CnDeviceBaseItem.h"
#include <device/CnDevice.h>


/*! \brief Base settings `item reference` class for a device/PLC.

    \details Settings are intended to change server (driver) operation parameters at runtime,
    unlike server configuration values, including programmatically.\n
    Combines common properties inherent to all settings,
    for example, all setting variables are writable (in addition to readable), i.e.
    the `AccessRights()` method returns the combination of flags OPC_READABLE | OPC_WRITEABLE.
*/

class CnDeviceSetItem : public CnDeviceBaseItem
{
public:
    using CnDeviceBaseItem::CnDeviceBaseItem;

public:
    /// \details Returns the `CnDeviceBaseItem::ItemType` of this ItemReference.
    ItemType itemType() const override { return Item_Setting; }

    /// \details  The `access()` method returns the flags combination `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }
};


/*! \brief Template settings `item reference` class for a device/PLC.

    \details Generates code common to all setting variables.\n
    Differs only by `CnDevice` class methods that return and write values of the corresponding setting element.\n
    Pointers to read and write methods are passed as template parameters.\n
    Reading settings is implemented in `ReadValue()`.\n
    Writing settings is implemented in `WriteValue()`.\n
    The class method responsible for returning a value in `ReadValue()` has the form `R CnDevice::PMethodRead()`,
    which is passed as a template parameter as a pointer to method `R (CnDevice::*PMethodRead)()`.\n
    The class method responsible for writing a value in `WriteValue()` has the form `void CnDevice::PMethodWrite(R)`,
    which is passed as a template parameter as a pointer to method `void (CnDevice::*PMethodWrite)(R)`.\n
*/
template <class R, R(CnDevice::*PMethodRead)() const, void(CnDevice::*PMethodWrite)(R)>
class CnDeviceSetItemBase : public CnDeviceSetItem
{
public:
    using CnDeviceSetItem::CnDeviceSetItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details  Implements reading the setting value by calling `CnDevice::*PMethodRead()`,
    /// a pointer to which is passed as the template parameter.
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        R v = (m_device->*PMethodRead)();
        //if (status)
        //    *status = Cn::Status_Good;
        //if (timestamp)
        //    *timestamp = Cn::currentTimestamp();
        return CnVariant(v);
    }

    /// \details  Implements writing the setting value by calling `CnDevice::*PMethodWrite()`,
    /// a pointer to which is passed as the template parameter.
    void update(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) override
    {
        R v;
        if (Cn::fromVariant(value, &v))
            (m_device->*PMethodWrite)(v);
    }
};

/// \details Проміжний шаблон функції, єдиним призначення якого є визначення типу повернення функції.
template <class RT> inline RT getDeviceSetMethodRT(RT(CnDevice::*)()const) { return RT(); }

#define DEVICE_SET_ITEM_TYPE(name) typedef CnDeviceSetItemBase<decltype(getDeviceSetMethodRT(&CnDevice::Setting##name)), &CnDevice::Setting##name, &CnDevice::setSetting##name>

DEVICE_SET_ITEM_TYPE(EnableDevice  ) CnDeviceSetItemEnableDevice  ; /*! Setting. Availability of this device/PLC in the execution cycle (1-on, 0-off) */

#endif // CNDEVICESETITEM_H