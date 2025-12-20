/*!
 * \file   CnPortBaseItem.h
 * \brief  Base class for an `item reference` variable for a port
 *
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNPORTBASEITEM_H
#define CNPORTBASEITEM_H

#include <CnBaseItem.h>

class CnPort;

/*! \brief Base class for an `item reference` variable for a port.

    \details An 'item reference' variable represents a memory cell used
    to exchange data with the port object. It has a primitive data type
    such as integer, floating-point number, or string and is used to
    expose internal port variables to the upper-level system.
*/

class CnPortBaseItem : public CnBaseItem
{
public:
    enum ItemType
    {
        Item_None,
        Item_Command,
        Item_Setting,
        Item_Statistic,
        Item_Config
    };

public:

    /// \details Class constructor.
    /// \param[in]  port  Pointer to a `CnPort` object that stores all
    /// runtime parameters of the port
    /// \param[in]  name  Variable name (defined in the configurator field 'Item Reference')
    CnPortBaseItem(CnPort *port, const CnString &name = CnString());

public:
    /// \details Returns the `CnPortBaseItem::ItemType` of this item reference.
    virtual ItemType itemType() const = 0;

    /// \details Returns the pointer to the `CnPort` object passed in the constructor.
    inline CnPort *port() const { return m_port; }

protected:
    CnPort *m_port;
};

#endif // CNPORTBASESVIOITEM_H
