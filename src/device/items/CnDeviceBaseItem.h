/*!
 * \file   CnDeviceBaseItem.h
 * \brief  Base class of `item reference` variable for a device/PLC
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDEVICEBASEITEM_H
#define CNDEVICEBASEITEM_H

#include <CnBaseItem.h>

class CnDevice;

/*! \brief Base class of `item reference` variable for a device/PLC.

    \details An 'item reference' variable represents a memory cell intended for exchange with the device/PLC object.
    It has a primitive data type such as integer, floating-point number, or string and is used to transfer process values and
    internal device/PLC variables to the upper-level system.
*/

class CnDeviceBaseItem : public CnBaseItem
{
public:
    enum ItemType
    {
        Item_None                ,
        Item_Modbus              ,
        Item_Command             ,
        Item_Setting             ,
        Item_Statistic           ,
        Item_Config              
    };

public:
    /// \details Class constructor.
    /// \param[in]  device     Pointer to an object of type `CnDevice` that stores all device/PLC runtime parameters
    /// \param[in]  name       Variable name (set in the configurator field 'Item Reference')
    CnDeviceBaseItem(CnDevice *device, const CnString &name);

     /// \details Class constructor.
    /// \param[in]  device     Pointer to an object of type `CnDevice` that stores all device/PLC runtime parameters
    CnDeviceBaseItem(CnDevice *device);

public:
    /// \details Returns the `CnDeviceBaseItem::ItemType` of this ItemReference.
    virtual ItemType itemType() const = 0;

    /// \details Returns the pointer to the `CnDevice` object passed in the constructor, which stores all device/PLC runtime parameters.
    inline CnDevice *device() const { return m_device; }

protected:
    CnDevice *m_device;
};

#endif // CNDEVICEBASEITEM_H
