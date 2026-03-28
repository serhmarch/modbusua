/*!
 * \file   CnPortStatItem.h
 * \brief  Defines the list of statistics/diagnostics `item reference` for a port
 *
 * \author serhmarch
 * \date   February 2024
 */
#ifndef CNPORTSTATISTICSVIOITEM_H
#define CNPORTSTATISTICSVIOITEM_H

#include "CnPortBaseItem.h"
#include <port/CnPort.h>
#include <port/CnPortStat.h>

 /*! \brief Base class for statistics/diagnostics `item reference` for a port.

     \details Combines common properties inherent to all stats/diagnostics
     variables; for example, they are read-only, i.e. `access()`
     returns the flag Cn::Access_Read.
 */

class CnPortStatItem : public CnPortBaseItem
{
public:
    CnPortStatItem(CnPort *port) : CnPortBaseItem(port), m_stat(port->stat()) {};

public:
    /// \details Returns the `CnDeviceBaseItem::ItemType` of this item reference.
    ItemType itemType() const override { return Item_Statistic; }

protected:
    CnPortStat *m_stat;
};

/*! \brief Templated statistics/diagnostics `item reference` for a port.

    \details Generates code common to all stats/diagnostics variables.
    It differs only by the class method that returns the value of the
    corresponding element. Reading is implemented in `ReadValue()`.
    The method used to return a value has the form `R CnPort::PMethod()`,
    passed as a template parameter as a pointer to `R (CnPort::*PMethod)()`.
*/

template <class R, R (CnPortStat::*PMethod)() const>
class CnPortStatItemBase : public CnPortStatItem
{
public:
    using CnPortStatItem::CnPortStatItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details Implements reading the statistics/diagnostics value by
    /// calling method `CnPort::*PMethod()`, the pointer to which is
    /// passed as a template parameter.
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

/// \details Intermediate function template whose sole purpose is to
/// determine the return type of the function.
template <class RT> inline RT getPortStatMethodRT(RT(CnPortStat::*)()const) { return RT(); }

#define PORT_STAT_ITEM_TYPE(name) typedef CnPortStatItemBase<decltype(getPortStatMethodRT(&CnPortStat::Stat##name)), &CnPortStat::Stat##name>

PORT_STAT_ITEM_TYPE(SinceTimestamp      ) CnPortStatItemSinceTimestamp      ; //!< Statistics. Timestamp since statistics is collected (after init or clear)
PORT_STAT_ITEM_TYPE(State               ) CnPortStatItemState               ; //!< Statistics. Bitmask of the current port state
PORT_STAT_ITEM_TYPE(StateEnablePort     ) CnPortStatItemStateEnablePort     ; //!< Statistics. Communication allowed (bit 0 of Stat.State)
PORT_STAT_ITEM_TYPE(ThreadCycleCount    ) CnPortStatItemThreadCycleCount    ; //!< Statistics. Thread execution: number of cycles since statistics started
PORT_STAT_ITEM_TYPE(ThreadLastCycle     ) CnPortStatItemThreadLastCycle     ; //!< Statistics. Thread execution: duration of last cycle (microseconds)
PORT_STAT_ITEM_TYPE(ThreadMinCycle      ) CnPortStatItemThreadMinCycle      ; //!< Statistics. Thread execution: minimum cycle time since start (microseconds)
PORT_STAT_ITEM_TYPE(ThreadMaxCycle      ) CnPortStatItemThreadMaxCycle      ; //!< Statistics. Thread execution: maximum cycle time since start (microseconds)
PORT_STAT_ITEM_TYPE(ThreadAvgCycle      ) CnPortStatItemThreadAvgCycle      ; //!< Statistics. Thread execution: average cycle time since start (microseconds)
PORT_STAT_ITEM_TYPE(GoodCount           ) CnPortStatItemGoodCount           ; //!< Statistics. Total number of successful Modbus requests
PORT_STAT_ITEM_TYPE(BadCount            ) CnPortStatItemBadCount            ; //!< Statistics. Total number of failed Modbus requests
PORT_STAT_ITEM_TYPE(Status              ) CnPortStatItemStatus              ; //!< Statistics. Status of the last Modbus request (sec. 2.9)
PORT_STAT_ITEM_TYPE(Timestamp           ) CnPortStatItemTimestamp           ; //!< Statistics. Time (PC) of the last Modbus request
PORT_STAT_ITEM_TYPE(LastSuccessTimestamp) CnPortStatItemLastSuccessTimestamp; //!< Statistics. Time (PC) of the last successful Modbus request
PORT_STAT_ITEM_TYPE(LastErrorStatus     ) CnPortStatItemLastErrorStatus     ; //!< Statistics. Status of the last Modbus error (sec. 2.9)
PORT_STAT_ITEM_TYPE(LastErrorTimestamp  ) CnPortStatItemLastErrorTimestamp  ; //!< Statistics. Time (PC) of the last failed Modbus request
PORT_STAT_ITEM_TYPE(LastErrorText       ) CnPortStatItemLastErrorText       ; //!< Statistics. Text representation of the last Modbus error

#endif // CNPORTSTATISTICSVIOITEM_H