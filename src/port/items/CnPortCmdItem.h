/*!
 * \file   CnPortCmdItem.h
 * \brief  Defines the list of command `item reference` for a port
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNPORTCOMMANDSVIOITEM_H
#define CNPORTCOMMANDSVIOITEM_H

#include "CnPortBaseItem.h"
#include <port/CnPort.h>

/*! \brief Base class for command `item reference` for a port.

    \details Combines common properties inherent to all commands,
    for example command variables are writable (and readable), i.e.
    the `access()` method returns Cn::Access_Read | OPC_WRITEABLE.
*/

class CnPortCmdItem : public CnPortBaseItem
{
public:
    using CnPortBaseItem::CnPortBaseItem;

public:
    /// \details Returns the `ItemType` of this item reference.
    ItemType itemType() const override { return Item_Command; }

    /// \details The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }

    /// \details Reading the command value always returns `false`.
    CnVariant value(Cn::StatusCode*, CnTimestamp*) const override
    {
        return CnVariant(false);
    }
};

// ------------------------------------------------------------------------------------------
// ------------------------------------ CnPortCmdItemBool -----------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief Templated boolean command `item reference` for a port.

    \details Generates code common to all commands. It differs only by
    the `CnPort` method that implements the command. Command execution
    is implemented in `WriteValue()`.
    The class method implementing the command has the form
    `void CnPort::PMethodWrite()`, passed as a template parameter as a
    pointer to method `void (CnPort::*PMethodWrite)()`.
*/

template <void(CnPort::*PMethodWrite)()>
class CnPortCmdItemBool : public CnPortCmdItem
{
public:
    using CnPortCmdItem::CnPortCmdItem;

public:
    /// \details For a boolean command the function returns type `Data_Bool`.
    Cn::DataType dataType() const override { return Cn::Data_Bool; }

    /// \details Executes the command by invoking `CnPort::*PMethodWrite()`,
    /// the pointer to which is passed as a template parameter.
    void update(const CnVariant &value, Cn::StatusCode, CnTimestamp) override
    {
        bool cmd = value.toBool();
        if (cmd)
            (m_port->*PMethodWrite)();
    }
};

#define PORT_CMD_ITEM_TYPE(name) typedef CnPortCmdItemBool<&CnPort::Cmd##name>

PORT_CMD_ITEM_TYPE(StatClear) CnPortCmdItemStatClear; //!< Command. Clear statistical information

#endif // CNCOMMANDITEM_H
