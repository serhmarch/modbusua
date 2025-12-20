/*!
 * \file   CnAppBaseItem.h
 * \brief  Base class of `item reference` variable for a port
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNSYSTEMBASEITEM_H
#define CNSYSTEMBASEITEM_H

#include "CnBaseItem.h"

class CnApp;

/*! \brief Base class of `item reference` variable for a device/PLC.

    \details An 'item reference' variable represents a memory cell intended for exchange with the port object.
    It has a primitive data type such as integer, floating-point number, or string and is used to pass internal port variables to the upper-level system.
*/

class CnAppBaseItem : public CnBaseItem
{
public:
    enum ItemType
    {
        Item_None   ,
        Item_Command,
        Item_Config 
    };

public:
    /// \details Class constructor.
    /// \param[in]  system    Pointer to an object of type `CnApp` that stores all port parameters during program execution
    /// \param[in]  name      Variable name (set in the configurator field 'Item Reference')
    CnAppBaseItem(CnApp *system, const CnString &name = CnString());

public:
    /// \details Returns the `CnAppBaseItem::ItemType` of this ItemReference.
    virtual ItemType itemType() const = 0;

    /// \details Returns the pointer to the `CnApp` object passed in the constructor, which stores all port parameters during program execution.
    inline CnApp *system() const { return m_system; }

protected:
    CnApp *m_system;
};

#endif // CNSYSTEMBASEITEM_H
