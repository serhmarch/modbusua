/*!
 * \file   CnDeviceStatItem.h
 * \brief  Defines the list of statistics/diagnostics `item reference` for device/PLC
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNDEVICESTATITEM_H
#define CNDEVICESTATITEM_H

#include "CnDeviceBaseItem.h"
#include <device/CnDevice.h>
#include <device/CnDeviceStat.h>


/*! \brief Base class for statistics/diagnostics `item reference` for device/PLC.

    \details Combines all common properties inherent to all statistics/diagnostics variables,
    for example all statistics/diagnostics variables are available for read-only access, i.e.
    the `AccessRights()` method returns the OPC_READABLE flag.
 */

class CnDeviceStatItem : public CnDeviceBaseItem
{
public:
    CnDeviceStatItem(CnDevice *device) : CnDeviceBaseItem(device), m_stat(device->stat()) {};

public:
    /// \details Function returns the `CnDeviceBaseItem::ItemType` type of this ItemReference.
    ItemType itemType() const override { return Item_Statistic; }

protected:
    CnDeviceStat *m_stat;
};


/*! \brief Template class for statistics/diagnostics `item reference` for device/PLC.

    \details Generates code common to all statistics/diagnostics variables.
    Differs only by the `CnDevice` class method that returns the value of the corresponding statistics/diagnostics element.
    The statistics/diagnostics reading code is implemented in the `ReadValue()` method.\n
    The class method responsible for returning the value in the `ReadValue()` method has the form `R CnDevice::PMethod()`,
    which is passed as a template parameter as a pointer to method `R (CnDevice::*PMethod)()`.
 */

template <class R, R (CnDeviceStat::*PMethod)() const>
class CnDeviceStatItemBase : public CnDeviceStatItem
{
public:
    using CnDeviceStatItem::CnDeviceStatItem;

public:
    /// \details Function returns the corresponding `Cn::DataType` type for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details  Implements execution of statistics/diagnostics value reading by calling the `CnDevice::*PMethod()` method, 
    /// pointer to which is passed as a template parameter.
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        R v = (m_stat->*PMethod)();
        //if (status)
        //    *status = Cn::Status_Good;
        //if (timestamp)
        //    *timestamp = Cn::currentTimestamp();
        return CnVariant(v);
    }
};

/// \details Intermediate function template whose sole purpose is to determine the function return type.
template <class RT> inline RT getDeviceStatMethodRT(RT(CnDeviceStat::*)()const) { return RT(); }

#define DEVICE_STAT_ITEM_TYPE(name) typedef CnDeviceStatItemBase<decltype(getDeviceStatMethodRT(&CnDeviceStat::Stat##name)), &CnDeviceStat::Stat##name>

DEVICE_STAT_ITEM_TYPE(State                             ) CnDeviceStatItemState                             ; //!< Statistics. Bit array of current gateway state
DEVICE_STAT_ITEM_TYPE(StateEnableDevice                 ) CnDeviceStatItemStateEnableDevice                 ; //!< Statistics. Communication allowed (corresponds to bit 0 of Stat.State)
DEVICE_STAT_ITEM_TYPE(StateConnectionAlive              ) CnDeviceStatItemStateConnectionAlive              ; //!< Statistics. Connection to PLC exists (corresponds to bit 2 of Stat.State)
DEVICE_STAT_ITEM_TYPE(StateIncluded                     ) CnDeviceStatItemStateIncluded                     ; //!< Statistics. This PLC is included in execution sequence. It can be excluded if communication error occurred and restore timeout (RestoreTimeout) is not yet completed (corresponds to bit 10 of Stat.State)
DEVICE_STAT_ITEM_TYPE(CommonGoodCount                   ) CnDeviceStatItemCommonGoodCount                   ; //!< Statistics. Total count of successful Modbus requests
DEVICE_STAT_ITEM_TYPE(CommonBadCount                    ) CnDeviceStatItemCommonBadCount                    ; //!< Statistics. Total count of failed Modbus requests
DEVICE_STAT_ITEM_TYPE(CommonStatus                      ) CnDeviceStatItemCommonStatus                      ; //!< Statistics. Status of last Modbus request (section 2.9)
DEVICE_STAT_ITEM_TYPE(CommonTimestamp                   ) CnDeviceStatItemCommonTimestamp                   ; //!< Statistics. Time (PC) of last Modbus request
DEVICE_STAT_ITEM_TYPE(CommonLastSuccessTimestamp        ) CnDeviceStatItemCommonLastSuccessTimestamp        ; //!< Statistics. Time (PC) of last successful Modbus request
DEVICE_STAT_ITEM_TYPE(CommonLastErrorStatus             ) CnDeviceStatItemCommonLastErrorStatus             ; //!< Statistics. Status of last Modbus request error (section 2.9)
DEVICE_STAT_ITEM_TYPE(CommonLastErrorTimestamp          ) CnDeviceStatItemCommonLastErrorTimestamp          ; //!< Statistics. Time (PC) of last failed Modbus request
DEVICE_STAT_ITEM_TYPE(CommonLastErrorText               ) CnDeviceStatItemCommonLastErrorText               ; //!< Statistics. Text representation of last Modbus request error 
DEVICE_STAT_ITEM_TYPE(PokeGoodCount                     ) CnDeviceStatItemPokeGoodCount                     ; //!< Statistics. Count of successful simple Modbus-Write requests
DEVICE_STAT_ITEM_TYPE(PokeBadCount                      ) CnDeviceStatItemPokeBadCount                      ; //!< Statistics. Count of failed simple Modbus-Write requests
DEVICE_STAT_ITEM_TYPE(PokeStatus                        ) CnDeviceStatItemPokeStatus                        ; //!< Statistics. Status of last simple Modbus-Write request (section 2.9)
DEVICE_STAT_ITEM_TYPE(PokeTimestamp                     ) CnDeviceStatItemPokeTimestamp                     ; //!< Statistics. Time (PC) of last simple Modbus-Write request
DEVICE_STAT_ITEM_TYPE(PokeLastSuccessTimestamp          ) CnDeviceStatItemPokeLastSuccessTimestamp          ; //!< Statistics. Time (PC) of last successful simple Modbus-Write request
DEVICE_STAT_ITEM_TYPE(PokeLastErrorStatus               ) CnDeviceStatItemPokeLastErrorStatus               ; //!< Statistics. Status of last simple Modbus-Write request error (section 2.9)
DEVICE_STAT_ITEM_TYPE(PokeLastErrorTimestamp            ) CnDeviceStatItemPokeLastErrorTimestamp            ; //!< Statistics. Time (PC) of last failed simple Modbus-Write request
DEVICE_STAT_ITEM_TYPE(PokeLastErrorText                 ) CnDeviceStatItemPokeLastErrorText                 ; //!< Statistics. Text representation of last simple Modbus-Write request error 
DEVICE_STAT_ITEM_TYPE(PollGoodCount                     ) CnDeviceStatItemPollGoodCount                     ; //!< Statistics. Count of successful simple Modbus-Read requests
DEVICE_STAT_ITEM_TYPE(PollBadCount                      ) CnDeviceStatItemPollBadCount                      ; //!< Statistics. Count of failed simple Modbus-Read requests
DEVICE_STAT_ITEM_TYPE(PollStatus                        ) CnDeviceStatItemPollStatus                        ; //!< Statistics. Status of last simple Modbus-Read request (section 2.9)
DEVICE_STAT_ITEM_TYPE(PollTimestamp                     ) CnDeviceStatItemPollTimestamp                     ; //!< Statistics. Time (PC) of last simple Modbus-Read request
DEVICE_STAT_ITEM_TYPE(PollLastSuccessTimestamp          ) CnDeviceStatItemPollLastSuccessTimestamp          ; //!< Statistics. Time (PC) of last successful simple Modbus-Read request
DEVICE_STAT_ITEM_TYPE(PollLastErrorStatus               ) CnDeviceStatItemPollLastErrorStatus               ; //!< Statistics. Status of last simple Modbus-Read request error (section 2.9)
DEVICE_STAT_ITEM_TYPE(PollLastErrorTimestamp            ) CnDeviceStatItemPollLastErrorTimestamp            ; //!< Statistics. Time (PC) of last failed simple Modbus-Read request
DEVICE_STAT_ITEM_TYPE(PollLastErrorText                 ) CnDeviceStatItemPollLastErrorText                 ; //!< Statistics. Text representation of last simple Modbus-Read request error
DEVICE_STAT_ITEM_TYPE(InnerState                        ) CnDeviceStatItemInnerState                        ; //!< Statistics. Internal state of communication algorithm */

#endif // CNDEVICESTATITEM_H