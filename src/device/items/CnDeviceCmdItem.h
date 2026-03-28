/*! 
 * \file   CnDeviceCmdItem.h
 * \brief  Defines command `item reference` list for a device/PLC
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNDEVICECMDITEM_H
#define CNDEVICECMDITEM_H

#include "CnDeviceBaseItem.h"
#include <device/CnDevice.h>

/*! \brief Base command `item reference` class for a device/PLC.

    \details Combines common properties inherent to all commands,
    for example, all command variables are writable (in addition to readable), i.e.
    the `access()` method returns the combination of flags Cn::Access_Read | OPC_WRITEABLE.
*/

class CnDeviceCmdItem : public CnDeviceBaseItem
{
public:
    using CnDeviceBaseItem::CnDeviceBaseItem;

public:
    /// \details Returns the `ItemType` of this ItemReference.
    ItemType itemType() const override { return Item_Command; }

    /// \details  The `access()` method returns the flags combination `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details  Reading a command value always returns `false`.
    CnVariant value(Cn::StatusCode*, CnTimestamp*) const override
    {
        return CnVariant(false);
    }
};

// ------------------------------------------------------------------------------------------
// ----------------------------------- CnDeviceCmdItemBool ----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template class for boolean command `item reference` for a device/PLC.

    \details Generates code common to all commands.
    Differs only by the `CnDevice` class method that implements the corresponding command.
    The command invocation code is implemented in the `WriteValue` method.\n
    The class method that implements the command has the form `void CnDevice::PMethodWrite()`,
    which is passed as a template parameter as a pointer to method `void(CnDevice::*PMethodWrite)()`.
*/

template <void(CnDevice::*PMethodWrite)()>
class CnDeviceCmdItemBool : public CnDeviceCmdItem
{
public:
    using CnDeviceCmdItem::CnDeviceCmdItem;

public:
    /// \details  For a boolean command, returns `Data_Bool`.
    Cn::DataType dataType() const override { return Cn::Data_Bool; }

    /// \details  Executes the command by calling `CnDevice::*PMethodWrite()`, a pointer to which is passed as the template parameter.
    void update(const CnVariant &value, Cn::StatusCode, CnTimestamp) override
    {
        bool cmd = value.toBool();
        if (cmd)
            (m_device->*PMethodWrite)();
    }
};

#define DEVICE_CMD_ITEM_TYPE(name) typedef CnDeviceCmdItemBool<&CnDevice::Cmd##name>

DEVICE_CMD_ITEM_TYPE(StatClear         ) CnDeviceCmdItemStatClear         ; /*!< Command. Reset statistical information                        */ 

#endif // CNDEVICECMDITEM_H
