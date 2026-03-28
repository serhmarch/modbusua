/*!
 * \file   CnAppCmdItem.h
 * \brief  Defines the list of command `item reference`s for a port
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNAPPCMDITEM_H
#define CNAPPCMDITEM_H

#include "CnAppBaseItem.h"
#include <app/CnApp.h>

/*! \brief Base class `item reference` command for port.

    \details Combines all common properties inherent to all commands,
    for example, all command variables are writable (and readable), i.e.,
    the `access()` method returns the combination of flags Cn::Access_Read | OPC_WRITEABLE.
*/

class CnAppCmdItem : public CnAppBaseItem
{
public:
    using CnAppBaseItem::CnAppBaseItem;

public:
    /// \details Returns the `ItemType` of this ItemReference.
    ItemType itemType() const override { return Item_Command; }

    /// \details  The `access()` method returns the combination `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details  The implementation of the read method for a command always returns `false`.
    CnVariant value(Cn::StatusCode*, CnTimestamp*) const override
    {
        return CnVariant(false);
    }
};

// ------------------------------------------------------------------------------------------
// ------------------------------------ CnAppCmdItemBool ------------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Template class of boolean command `item reference` for a port.

    \details Generates code common to all commands.
    It differs only by the method of the `CnApp` class that implements the corresponding command.
    The command invocation code is implemented in the `WriteValue()` method.\n
    The class method responsible for executing the command has the form `void CnApp::PMethodWrite()`,
    which is passed as a template parameter as a pointer to the method `void(CnApp::*PMethodWrite)()`.
*/

template <void(CnApp::*PMethodWrite)()>
class CnAppCmdItemBool : public CnAppCmdItem
{
public:
    using CnAppCmdItem::CnAppCmdItem;

public:
    /// \details  For a boolean command, returns type `Data_Bool`.
    Cn::DataType dataType() const override { return Cn::Data_Bool; }

    /// \details  Executes the command by invoking `CnApp::*PMethodWrite()`,
    /// whose pointer is passed as a template parameter.
    void update(const CnVariant &value, Cn::StatusCode, CnTimestamp) override
    {
        bool cmd = value.toBool();
        if (cmd)
            (m_system->*PMethodWrite)();
    }
};

#define SYSTEM_CMD_ITEM_TYPE(name) typedef CnAppCmdItemBool<&CnApp::command##name>

SYSTEM_CMD_ITEM_TYPE(ReloadConfig) CnAppCmdItemReloadConfig; //!< Command. Reload configuration

#endif // CNAPPCMDITEM_H
