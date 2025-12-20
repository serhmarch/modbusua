/*!
 * \file   CnAppCfgItem.h
 * \brief  Defines the list of configuration `item reference`s for a port
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNSYSTEMCFGITEM_H
#define CNSYSTEMCFGITEM_H

#include "CnAppBaseItem.h"
#include <app/CnApp.h>
#include <app/CnAppConfig.h>


 /*! \brief Base class for configuration `item reference` for a port.

     \details Combines common properties inherent to all configuration variables;
     for example, all configuration variables are read-only, i.e.,
     the `AccessRights()` method returns the OPC_READABLE flag.
 */

class CnAppCfgItem : public CnAppBaseItem
{
public:
    CnAppCfgItem(CnApp *system) : CnAppBaseItem(system), m_cfg(system->cfg()) {};

public:
    /// \details Returns the `ItemType` of this ItemReference.
    ItemType itemType() const override { return Item_Config; }

protected:
    CnAppConfig *m_cfg;
};

/*! \brief Template class of configuration `item reference` for a port.

    \details Generates code common to all configuration variables.
    It differs only by the `CnAppConfig` method that returns the value of the corresponding configuration element.
    The configuration reading code is implemented in the `ReadValue()` method.\n
    The class method responsible for returning the value in `ReadValue()` has the form `R CnAppConfig::PMethod()`,
    which is passed as a template parameter as a pointer to the method `R (CnAppConfig::*PMethod)()`.
*/

template <class R, R (CnAppConfig::*PMethod)() const>
class CnAppCfgItemBase : public CnAppCfgItem
{
public:
    using CnAppCfgItem::CnAppCfgItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details  Implements reading the configuration value by invoking `CnAppConfig::*PMethod()`,
    /// whose pointer is passed as a template parameter.
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

/// \details Intermediate function template whose sole purpose is to determine the function's return type.
template <class RT> inline RT getSystemCfgMethodRT(RT(CnAppConfig::*)()const) { return RT(); }

#define SYSTEM_CFG_ITEM_TYPE(name) typedef CnAppCfgItemBase<decltype(getSystemCfgMethodRT(&CnAppConfig::Cfg##name)), &CnAppConfig::Cfg##name>

SYSTEM_CFG_ITEM_TYPE(LogFlags            ) CnAppCfgItemLogFlags            ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogOutput           ) CnAppCfgItemLogOutput           ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogDefaultFormat    ) CnAppCfgItemLogDefaultFormat    ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogDefaultTimeformat) CnAppCfgItemLogDefaultTimeformat;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogSystemFormat     ) CnAppCfgItemLogSystemFormat     ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogSystemTimeformat ) CnAppCfgItemLogSystemTimeformat ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogConsoleFormat    ) CnAppCfgItemLogConsoleFormat    ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogConsoleTimeformat) CnAppCfgItemLogConsoleTimeformat;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogFileFormat       ) CnAppCfgItemLogFileFormat       ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogFileTimeformat   ) CnAppCfgItemLogFileTimeformat   ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogFilePath         ) CnAppCfgItemLogFilePath         ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogFileMaxCount     ) CnAppCfgItemLogFileMaxCount     ;  //!< Configuration.
SYSTEM_CFG_ITEM_TYPE(LogFileMaxSize      ) CnAppCfgItemLogFileMaxSize      ;  //!< Configuration.

#endif // CNSYSTEMCFGITEM_H