/*!
 * \file   CnPortCfgItem.h
 * \brief  Defines the list of configuration `item reference` for a port
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNPORTCFGITEM_H
#define CNPORTCFGITEM_H

#include "CnPortBaseItem.h"
#include <port/CnPort.h>
#include <port/CnPortConfig.h>


 /*! \brief Base class for configuration `item reference` for a port.

     \details Combines common properties inherent to all configuration
     variables, for example configuration variables are read-only, i.e.
     the `access()` method returns the flag Cn::Access_Read.
 */

class CnPortCfgItem : public CnPortBaseItem
{
public:
    CnPortCfgItem(CnPort *port) : CnPortBaseItem(port), m_cfg(port->cfg()) {};

public:
    /// \details Returns the `ItemType` of this item reference.
    ItemType itemType() const override { return Item_Config; }

protected:
    CnPortConfig *m_cfg;
};

/*! \brief Templated configuration `item reference` for a port.

    \details Generates code common to all configuration variables.
    It differs only by the class method that returns the value of the
    corresponding configuration element. Reading configuration is
    implemented in `ReadValue()`.
    The class method used to return a value in `ReadValue()` has the
    form `R CnPort::PMethod()`, passed as a template parameter as a
    pointer to method `R (CnPort::*PMethod)()`.
*/

template <class R, R (CnPortConfig::*PMethod)() const>
class CnPortCfgItemBase : public CnPortCfgItem
{
public:
    using CnPortCfgItem::CnPortCfgItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details Implements reading the configuration value by calling
    /// method `CnPort::*PMethod()`, the pointer to which is passed as a
    /// template parameter.
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

/// \details Intermediate function template whose sole purpose is to
/// determine the return type of the function.
template <class RT> inline RT getPortCfgMethodRT(RT(CnPortConfig::*)()const) { return RT(); }

#define PORT_CFG_ITEM_TYPE(name) typedef CnPortCfgItemBase<decltype(getPortCfgMethodRT(&CnPortConfig::Cfg##name)), &CnPortConfig::Cfg##name>

PORT_CFG_ITEM_TYPE(Name             ) CnPortCfgItemName             ; //!< Configuration. Name of this port
PORT_CFG_ITEM_TYPE(Enable           ) CnPortCfgItemEnable           ; //!< Configuration. Availability for runtime execution sequence (1-on, 0-off)
PORT_CFG_ITEM_TYPE(Type             ) CnPortCfgItemType             ; //!< Configuration. Modbus protocol type
PORT_CFG_ITEM_TYPE(Host             ) CnPortCfgItemHost             ; //!< Configuration. IP address or DNS name of the remote device
PORT_CFG_ITEM_TYPE(Port             ) CnPortCfgItemPort             ; //!< Configuration. TCP port number (standard Modbus TCP port 502)
PORT_CFG_ITEM_TYPE(Timeout          ) CnPortCfgItemTimeout          ; //!< Configuration. Connection timeout (milliseconds)
PORT_CFG_ITEM_TYPE(SerialPortName   ) CnPortCfgItemSerialPortName   ; //!< Configuration. Serial port name
PORT_CFG_ITEM_TYPE(BaudRate         ) CnPortCfgItemBaudRate         ; //!< Configuration. Serial port baud rate
PORT_CFG_ITEM_TYPE(DataBits         ) CnPortCfgItemDataBits         ; //!< Configuration. Serial port data bits
PORT_CFG_ITEM_TYPE(Parity           ) CnPortCfgItemParity           ; //!< Configuration. Serial port parity
PORT_CFG_ITEM_TYPE(StopBits         ) CnPortCfgItemStopBits         ; //!< Configuration. Serial port stop bits
PORT_CFG_ITEM_TYPE(FlowControl      ) CnPortCfgItemFlowControl      ; //!< Configuration. Serial port flow control
PORT_CFG_ITEM_TYPE(TimeoutFirstByte ) CnPortCfgItemTimeoutFirstByte ; //!< Configuration. Timeout waiting for the first byte (ms)
PORT_CFG_ITEM_TYPE(TimeoutInterByte ) CnPortCfgItemTimeoutInterByte ; //!< Configuration. Inter-byte timeout (ms)
PORT_CFG_ITEM_TYPE(LogFlags         ) CnPortCfgItemLogFlags         ; //!< Configuration. Logging flags
PORT_CFG_ITEM_TYPE(LogOutput        ) CnPortCfgItemLogOutput        ; //!< Configuration. Logging output target
PORT_CFG_ITEM_TYPE(LogFileFormat    ) CnPortCfgItemLogFileFormat    ; //!< Configuration. Log file message format
PORT_CFG_ITEM_TYPE(LogFileTimeformat) CnPortCfgItemLogFileTimeformat; //!< Configuration. Log file time format
PORT_CFG_ITEM_TYPE(LogFilePath      ) CnPortCfgItemLogFilePath      ; //!< Configuration. Log file path
PORT_CFG_ITEM_TYPE(LogFileMaxCount  ) CnPortCfgItemLogFileMaxCount  ; //!< Configuration. Max number of rotated log files
PORT_CFG_ITEM_TYPE(LogFileMaxSize   ) CnPortCfgItemLogFileMaxSize   ; //!< Configuration. Max size of a single log file

#endif // CNPORTCFGITEM_H