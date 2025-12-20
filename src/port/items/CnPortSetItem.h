/*!
 * \file   CnPortSetItem.h
 * \brief  Defines the list of settings `item reference` for a port
 *
 * \author serhmarch
 * \date   February 2024
 */
#ifndef CNPORTSETTINGSVIOITEM_H
#define CNPORTSETTINGSVIOITEM_H

#include "CnPortBaseItem.h"
#include <port/CnPort.h>

/*! \brief Base class for settings `item reference` for a port.

    \details Settings are intended to change server (driver) parameters
    at runtime, unlike configuration values, including programmatically.
    Combines common properties inherent to all settings: settings
    variables are writable (and readable), i.e. `AccessRights()` returns
    the combination OPC_READABLE | OPC_WRITEABLE.
 */

class CnPortSetItem : public CnPortBaseItem
{
public:
    using CnPortBaseItem::CnPortBaseItem;

public:
    /// \details Returns the `CnPortBaseItem::ItemType` of this item reference.
    ItemType itemType() const override { return Item_Setting; }

    /// \details The `access()` method returns `Cn::Access_ReadWrite`.
    Cn::Access access() const override { return Cn::Access_ReadWrite; }
};

/*! \brief Templated settings `item reference` for a port.

    \details Generates code common to all settings variables.
    It differs only by the `CnPort` methods that return and write values
    of the corresponding settings element.
    Template parameters are pointers to reading and writing methods.
    Reading settings is implemented in `ReadValue()`; writing settings
    is implemented in `WriteValue()`.
    The class method used to return a value in `ReadValue()` has the
    form `R CnPort::PMethodRead()`, passed as a template parameter as a
    pointer to `R (CnPort::*PMethodRead)()`.
    The class method used to write a value in `WriteValue()` has the
    form `void CnPort::PMethodWrite(R)`, passed as a template parameter
    as a pointer to `void (CnPort::*PMethodWrite)(R)`.
*/

template <class R, R(CnPort::*PMethodRead)() const, void(CnPort::*PMethodWrite)(R)>
class CnPortSetItemBase : public CnPortSetItem
{
public:
    using CnPortSetItem::CnPortSetItem;

public:
    /// \details Returns the corresponding `Cn::DataType` for the variable.
    Cn::DataType dataType() const override { return Cn::toDataType<R>(); }

    /// \details Implements reading the settings value by calling
    /// `CnPort::*PMethodRead()`, the pointer to which is passed as a
    /// template parameter.
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        R v = (m_port->*PMethodRead)();
        //if (status)
        //    *status = Cn::Status_Good;
        //if (timestamp)
        //    *timestamp = Cn::currentTimestamp();
        return CnVariant(v);
    }

    /// \details Implements writing the settings value by calling
    /// `CnPort::*PMethodWrite()`, the pointer to which is passed as a
    /// template parameter.
    void update(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp) override
    {
        R v;
        if (Cn::fromVariant(value, &v))
            (m_port->*PMethodWrite)(v);
    }
};

/// \details Intermediate function template whose sole purpose is to
/// determine the return type of the function.
template <class RT> inline RT getPortSetMethodRT(RT(CnPort::*)()const) { return RT(); }

#define PORT_SET_ITEM_TYPE(name) typedef CnPortSetItemBase<decltype(getPortSetMethodRT(&CnPort::Setting##name)), &CnPort::Setting##name, &CnPort::setSetting##name>

PORT_SET_ITEM_TYPE(Enable) CnPortSetItemEnable; //!< Setting. Port availability for the execution loop (1-available, 0-unavailable).

#endif // CNPORTSETTINGSVIOITEM_H