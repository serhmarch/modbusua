/*!\
 * \file   CnBaseItem.h\
 * \brief  Base class for the `item reference` variable in the gateway
 *\
 * \author serhmarch\
 * \date   December 2024\
 */
#ifndef CNBASEITEM_H
#define CNBASEITEM_H

#include <CnGlobal.h>
#include <CnObject.h>

/*! \brief Base class for the `item reference` variable in the gateway.

    \details The 'item reference' variable represents a memory cell intended for exchange between the upper-level system and the server (driver).
    It has a primitive data type, such as integer, floating-point number, or string, and is used to transfer values of the technological process and
    internal server (driver) variables to the upper-level system.
*/

class CnBaseItem
{
public:
    /// \details Class constructor. Takes the variable name as an argument (specified in the configurator field 'Item Reference').
    CnBaseItem(const CnString &name = CnString());

    /// \details Class destructor.
    virtual ~CnBaseItem();

public:
    /// \details Function returns the corresponding `Cn::DataType` type for the variable.
    virtual Cn::DataType dataType() const = 0;

    /// \details Function returns the corresponding access type `Cn::Access` for the variable.
    virtual Cn::Access access() const;

    /// \details Function returns the name of the current `item reference`.
    inline const CnString &name() const { return m_name; }

    /// \details Function sets the name of the current `item reference`.
    inline void setName(const CnString &name) { m_name = name; }

    /// \details Function sets the name of the current `item reference`.
    inline void setName(const CnChar *name) { m_name = name; }

    /// \details Function returns the description of the current `item reference`.
    inline const CnString &description() const { return m_descr; }

    /// \details Function sets the description of the current `item reference`.
    inline void setDescription(const CnString &descr) { m_descr = descr; }

    /// \details Function sets the description of the current `item reference`.
    inline void setDescription(const CnChar *descr) { m_descr = descr; }

    /// \details Function returns a pointer to any data structure that can be set for the current `item reference`.
    inline void *context() const { return m_context; }

    /// \details Function sets a pointer to any data structure for the current `item reference`.
    inline void setContext(void *context) { m_context = context; }

    /// \details Function returns the current value of the `item reference`.
    virtual CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const = 0;

    /// \details Function sets the current value of the `item reference`, as well as the status value and timestamp.
    virtual void update(const CnVariant &value, Cn::StatusCode status = Cn::Status_Good, CnTimestamp timestamp = CnTimestamp());

    /// \details Function sets only the status value and timestamp.
    inline void update(Cn::StatusCode status, CnTimestamp timestamp = CnTimestamp()) { update(CnVariant(), status, timestamp); }

    /// \details Function sets only the status value and timestamp.
    inline void updateStatusTimestamp(Cn::StatusCode status, CnTimestamp timestamp) { update(CnVariant(), status, timestamp); }

protected:
    mutable CnCriticalSection m_cs;
    CnString m_name;
    CnString m_descr;
    void *m_context;
};

#endif // CNBASEITEM_H
